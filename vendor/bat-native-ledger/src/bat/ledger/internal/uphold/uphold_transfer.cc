/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/uphold/uphold_transfer.h"

#include <utility>

#include "bat/ledger/global_constants.h"
#include "bat/ledger/internal/endpoints/request_for.h"
#include "bat/ledger/internal/ledger_impl.h"
#include "bat/ledger/internal/notifications/notification_keys.h"
#include "net/http/http_status_code.h"

using ledger::endpoints::PostCommitTransactionUphold;
using ledger::endpoints::PostCreateTransactionUphold;
using ledger::endpoints::RequestFor;

namespace ledger::uphold {

UpholdTransfer::UpholdTransfer(LedgerImpl* ledger) : ledger_(ledger) {
  DCHECK(ledger_);
}

UpholdTransfer::~UpholdTransfer() = default;

void UpholdTransfer::CreateTransaction(
    const std::string& destination,
    double amount,
    client::CreateTransactionCallback callback) const {
  const auto wallet =
      ledger_->uphold()->GetWalletIf({mojom::WalletStatus::kConnected});
  if (!wallet) {
    return callback("");
  }

  RequestFor<PostCreateTransactionUphold>(ledger_, std::move(wallet->token),
                                          std::move(wallet->address),
                                          std::string(destination), amount)
      .Send(base::BindOnce(&UpholdTransfer::OnCreateTransaction,
                           base::Unretained(this), std::move(callback)));
}

void UpholdTransfer::OnCreateTransaction(
    client::CreateTransactionCallback callback,
    PostCreateTransactionUphold::Result&& result) const {
  if (!ledger_->uphold()->GetWalletIf({mojom::WalletStatus::kConnected})) {
    return callback("");
  }

  if (!result.has_value()) {
    if (result.error() ==
        PostCreateTransactionUphold::Error::kAccessTokenExpired) {
      if (!ledger_->uphold()->LogOutWallet()) {
        BLOG(0,
             "Failed to disconnect " << constant::kWalletUphold << " wallet!");
      }
    }

    return callback("");
  }

  callback(std::move(result.value()));
}

void UpholdTransfer::CommitTransaction(
    const std::string& transaction_id,
    client::CommitTransactionCallback callback) const {
  const auto wallet =
      ledger_->uphold()->GetWalletIf({mojom::WalletStatus::kConnected});
  if (!wallet) {
    return callback(false);
  }

  RequestFor<PostCommitTransactionUphold>(ledger_, std::move(wallet->token),
                                          std::move(wallet->address),
                                          std::string(transaction_id))
      .Send(base::BindOnce(&UpholdTransfer::OnCommitTransaction,
                           base::Unretained(this), std::move(callback)));
}

void UpholdTransfer::OnCommitTransaction(
    client::CommitTransactionCallback callback,
    PostCommitTransactionUphold::Result&& result) const {
  if (!ledger_->uphold()->GetWalletIf({mojom::WalletStatus::kConnected})) {
    return callback(false);
  }

  if (!result.has_value()) {
    if (result.error() ==
        PostCommitTransactionUphold::Error::kAccessTokenExpired) {
      if (!ledger_->uphold()->LogOutWallet()) {
        BLOG(0,
             "Failed to disconnect " << constant::kWalletUphold << " wallet!");
      }
    }

    return callback(false);
  }

  callback(true);
}

}  // namespace ledger::uphold
