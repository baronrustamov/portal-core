/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ML_PIPELINE_TEXT_PROCESSING_EMBEDDING_INFO_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ML_PIPELINE_TEXT_PROCESSING_EMBEDDING_INFO_H_

#include <string>
#include <vector>

namespace ads::ml::pipeline {

struct TextEmbeddingInfo final {
  TextEmbeddingInfo();

  TextEmbeddingInfo(const TextEmbeddingInfo& other);
  TextEmbeddingInfo& operator=(const TextEmbeddingInfo& other);

  TextEmbeddingInfo(TextEmbeddingInfo&& other) noexcept;
  TextEmbeddingInfo& operator=(TextEmbeddingInfo&& other) noexcept;

  ~TextEmbeddingInfo();

  std::string text;
  std::string hashed_text_base64;
  std::string locale;
  std::vector<float> embedding;
};

}  // namespace ads::ml::pipeline

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ML_PIPELINE_TEXT_PROCESSING_EMBEDDING_INFO_H_
