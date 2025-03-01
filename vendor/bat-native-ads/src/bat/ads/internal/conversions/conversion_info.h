/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CONVERSIONS_CONVERSION_INFO_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CONVERSIONS_CONVERSION_INFO_H_

#include <string>
#include <vector>

#include "base/time/time.h"

namespace ads {

struct ConversionInfo final {
  ConversionInfo();

  ConversionInfo(const ConversionInfo& other);
  ConversionInfo& operator=(const ConversionInfo& other);

  ConversionInfo(ConversionInfo&& other) noexcept;
  ConversionInfo& operator=(ConversionInfo&& other) noexcept;

  ~ConversionInfo();

  bool IsValid() const;

  bool operator==(const ConversionInfo& other) const;
  bool operator!=(const ConversionInfo& other) const;

  std::string creative_set_id;
  std::string type;
  std::string url_pattern;
  std::string advertiser_public_key;
  int observation_window = 0;
  base::Time expire_at;
};

using ConversionList = std::vector<ConversionInfo>;

}  // namespace ads

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CONVERSIONS_CONVERSION_INFO_H_
