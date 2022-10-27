/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_SERVING_CHOOSE_SAMPLE_ADS_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_SERVING_CHOOSE_SAMPLE_ADS_H_

#include <ostream>
#include <vector>

#include "absl/types/optional.h"
#include "base/notreached.h"
#include "base/rand_util.h"
#include "bat/ads/internal/ads/serving/choose/ad_predictor_info.h"
#include "bat/ads/internal/base/numbers/number_util.h"

namespace ads {

template <typename T>
T CalculateNormalizingConstantBase(const std::vector<T>& scores) {
  T normalizing_constant = 0;
  for (const auto& score : scores) {
    normalizing_constant += score;
  }

  return normalizing_constant;
}

template <typename T>
double CalculateNormalizingConstant(
    const CreativeAdPredictorMap<T>& creative_ad_predictors) {
  std::vector<double> scores;
  scores.assign(creative_ad_predictors.size(), 0);

  for (const auto& creative_ad_predictor : creative_ad_predictors) {
    const AdPredictorInfo<T>& ad_predictor = creative_ad_predictor.second;
    scores.push_back(ad_predictor.score);
  }

  return CalculateNormalizingConstantBase(scores);
}

template <typename T>
absl::optional<T> SampleAdFromPredictors(
    const CreativeAdPredictorMap<T>& creative_ad_predictors) {
  const double normalizing_constant =
      CalculateNormalizingConstant(creative_ad_predictors);
  if (DoubleIsLessEqual(normalizing_constant, 0.0)) {
    return absl::nullopt;
  }

  const double rand = base::RandDouble();
  double sum = 0;

  for (const auto& creative_ad_predictor : creative_ad_predictors) {
    const AdPredictorInfo<T> ad_predictor = creative_ad_predictor.second;

    const double probability = ad_predictor.score / normalizing_constant;
    sum += probability;

    if (DoubleIsLess(rand, sum)) {
      return ad_predictor.creative_ad;
    }
  }

  NOTREACHED() << "Sum should always be less than probability";
  return absl::nullopt;
}

inline std::vector<double> ComputeProbabilities(
    const std::vector<int>& scores) {
  std::vector<double> probabilities;

  const double normalizing_constant = CalculateNormalizingConstantBase(scores);

  probabilities.reserve(scores.size());
  for (const auto& score : scores) {
    probabilities.push_back(score / normalizing_constant);
  }

  return probabilities;
}

}  // namespace ads

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_SERVING_CHOOSE_SAMPLE_ADS_H_
