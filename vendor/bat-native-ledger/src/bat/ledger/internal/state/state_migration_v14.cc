/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/state/state_migration_v14.h"

#include "bat/ledger/internal/ledger_impl.h"

namespace ledger::state {

StateMigrationV14::StateMigrationV14(LedgerImpl* ledger) : ledger_(ledger) {
  DCHECK(ledger_);
}

StateMigrationV14::~StateMigrationV14() = default;

void StateMigrationV14::Migrate(ledger::LegacyResultCallback callback) {
  callback(mojom::Result::LEDGER_OK);
}

}  // namespace ledger::state
