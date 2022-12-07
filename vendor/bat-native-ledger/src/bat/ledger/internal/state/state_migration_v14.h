/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_STATE_STATE_MIGRATION_V14_H_
#define BRAVE_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_STATE_STATE_MIGRATION_V14_H_

#include "bat/ledger/ledger.h"

namespace ledger {
class LedgerImpl;

namespace state {

class StateMigrationV14 {
 public:
  explicit StateMigrationV14(LedgerImpl*);
  ~StateMigrationV14();

  void Migrate(ledger::LegacyResultCallback);

 private:
  LedgerImpl* ledger_;  // NOT OWNED
};

}  // namespace state
}  // namespace ledger

#endif  // BRAVE_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_STATE_STATE_MIGRATION_V14_H_
