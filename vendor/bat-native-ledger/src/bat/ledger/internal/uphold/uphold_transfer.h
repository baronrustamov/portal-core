/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_UPHOLD_UPHOLD_TRANSFER_H_
#define BRAVE_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_UPHOLD_UPHOLD_TRANSFER_H_

#include <string>

#include "bat/ledger/internal/endpoints/uphold/post_commit_transaction/post_commit_transaction_uphold.h"
#include "bat/ledger/internal/endpoints/uphold/post_create_transaction/post_create_transaction_uphold.h"
#include "bat/ledger/internal/uphold/uphold.h"
#include "bat/ledger/ledger.h"

namespace ledger {
class LedgerImpl;

namespace uphold {

class UpholdTransfer final {
 public:
  explicit UpholdTransfer(LedgerImpl*);

  ~UpholdTransfer();

  void CreateTransaction(const std::string& destination,
                         double amount,
                         client::CreateTransactionCallback) const;

  void CommitTransaction(const std::string& transaction_id,
                         client::CommitTransactionCallback) const;

 private:
  void OnCreateTransaction(
      client::CreateTransactionCallback,
      endpoints::PostCreateTransactionUphold::Result&&) const;

  void OnCommitTransaction(
      client::CommitTransactionCallback,
      endpoints::PostCommitTransactionUphold::Result&&) const;

  LedgerImpl* ledger_;  // NOT OWNED
};

}  // namespace uphold
}  // namespace ledger

#endif  // BRAVE_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_UPHOLD_UPHOLD_TRANSFER_H_
