/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_ENDPOINT_UPHOLD_POST_CARDS_POST_CARDS_H_
#define BRAVE_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_ENDPOINT_UPHOLD_POST_CARDS_POST_CARDS_H_

#include <string>

#include "bat/ledger/ledger.h"

// POST https://api.uphold.com/v0/me/cards
//
// Request body:
// {
//   "label": "Portal browser",
//   "currency": "BAT"
// }
//
// Success code:
// HTTP_OK (200)
//
// Error codes:
// HTTP_UNAUTHORIZED (401)
//
// Response body:
// {
//   "CreatedByApplicationId": "193a77cf-02e8-4e10-8127-8a1b5a8bfece",
//   "address": {
//     "wire": "XXXXXXXXXX"
//   },
//   "available": "0.00",
//   "balance": "0.00",
//   "currency": "BAT",
//   "id": "bd91a720-f3f9-42f8-b2f5-19548004f6a7",
//   "label": "Portal browser",
//   "lastTransactionAt": null,
//   "settings": {
//     "position": 8,
//     "protected": false,
//     "starred": false
//   },
//   "createdByApplicationClientId": "4c2b665ca060d912fec5c735c734859a06118cc8",
//   "normalized": [
//     {
//       "available": "0.00",
//       "balance": "0.00",
//       "currency": "USD"
//     }
//   ],
//   "wire": [
//     {
//       "accountName": "Uphold Europe Limited",
//       "address": {
//         "line1": "Tartu mnt 2",
//         "line2": "10145 Tallinn, Estonia"
//       },
//       "bic": "LHVBEE22",
//       "currency": "EUR",
//       "iban": "EE76 7700 7710 0159 0178",
//       "name": "AS LHV Pank"
//     },
//     {
//       "accountName": "Uphold HQ, Inc.",
//       "accountNumber": "XXXXXXXXXX",
//       "address": {
//         "line1": "1359 Broadway",
//         "line2": "New York, NY 10018"
//       },
//       "bic": "MCBEUS33",
//       "currency": "USD",
//       "name": "Metropolitan Bank",
//       "routingNumber": "XXXXXXXXX"
//     }
//   ]
// }

namespace ledger {
class LedgerImpl;

namespace endpoint::uphold {

using PostCardsCallback =
    base::OnceCallback<void(mojom::Result, std::string&& id)>;

class PostCards {
 public:
  explicit PostCards(LedgerImpl*);
  ~PostCards();

  void Request(const std::string& token, PostCardsCallback);

 private:
  std::string GetUrl();

  std::string GeneratePayload();

  mojom::Result CheckStatusCode(int status_code);

  mojom::Result ParseBody(const std::string& body, std::string* id);

  void OnRequest(PostCardsCallback, const mojom::UrlResponse&);

  LedgerImpl* ledger_;  // NOT OWNED
};

}  // namespace endpoint::uphold
}  // namespace ledger

#endif  // BRAVE_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_ENDPOINT_UPHOLD_POST_CARDS_POST_CARDS_H_
