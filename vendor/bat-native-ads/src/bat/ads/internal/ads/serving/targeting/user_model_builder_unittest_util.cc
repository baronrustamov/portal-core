/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ads/serving/targeting/user_model_builder_unittest_util.h"

#include "bat/ads/internal/ads/serving/targeting/user_model_info.h"

namespace ads::targeting {

UserModelInfo BuildUserModel(const SegmentList& interest_segments,
                             const SegmentList& latent_interest_segments,
                             const SegmentList& purchase_intent_segments) {
  UserModelInfo user_model;
  user_model.interest_segments = interest_segments;
  user_model.latent_interest_segments = latent_interest_segments;
  user_model.purchase_intent_segments = purchase_intent_segments;
  return user_model;
}

}  // namespace ads::targeting
