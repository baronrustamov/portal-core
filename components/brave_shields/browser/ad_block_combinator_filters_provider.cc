/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_shields/browser/ad_block_combinator_filters_provider.h"

#include <memory>
#include <string>
#include <utility>

#include "base/logging.h"

namespace brave_shields {

AdBlockCombinatorFiltersProvider::AdBlockCombinatorFiltersProvider() {}

AdBlockCombinatorFiltersProvider::~AdBlockCombinatorFiltersProvider() = default;

void AdBlockCombinatorFiltersProvider::CombinateProvider(
    AdBlockFiltersProvider* provider) {
  auto it = std::find(combinated_providers_.begin(),
                      combinated_providers_.end(), provider);
  CHECK(it == combinated_providers_.end());
  combinated_providers_.push_back(provider);
  provider->AddObserver(this);
}

void AdBlockCombinatorFiltersProvider::UncombinateProvider(
    AdBlockFiltersProvider* provider) {
  auto it = std::find(combinated_providers_.begin(),
                      combinated_providers_.end(), provider);
  CHECK(it != combinated_providers_.end());
  (*it)->RemoveObserver(this);
  combinated_providers_.erase(it);
  AdBlockFiltersProvider::OnNewSourceAvailable();
}

void AdBlockCombinatorFiltersProvider::OnNewSourceAvailable(
    base::WeakPtr<AdBlockFiltersProvider> from) {
  AdBlockFiltersProvider::OnNewSourceAvailable();
}

void AdBlockCombinatorFiltersProvider::LoadDATBuffer(
    base::OnceCallback<void(bool deserialize, const DATFileDataBuffer& dat_buf)>
        cb) {
  request_nonce_ += 1;
  if (dats_to_load_ > 0) {
    // There's already an in-progress request, reset the combinator.
    // In-progress loads will be ignored by the new request nonce.
    combined_list_.clear();
  }
  dats_to_load_ = combinated_providers_.size();
  cb_ = std::move(cb);
  if (dats_to_load_ == 0) {
    FinishCombinating();
  } else {
    for (auto* provider : combinated_providers_) {
      provider->LoadDAT(
          base::BindOnce(&AdBlockCombinatorFiltersProvider::OnDATLoaded,
                         weak_factory_.GetWeakPtr(), request_nonce_));
    }
  }
}

void AdBlockCombinatorFiltersProvider::OnDATLoaded(
    int request_nonce,
    bool deserialize,
    const DATFileDataBuffer& dat_buf) {
  // This combinator should never be used with a provider that returns a
  // serialized DAT. The ability should be removed from the FiltersProvider API
  // when possible.
  CHECK(!deserialize);

  if (request_nonce != request_nonce_) {
    return;
  }

  dats_to_load_ -= 1;
  combined_list_.push_back('\n');
  combined_list_.insert(combined_list_.end(), dat_buf.begin(), dat_buf.end());

  if (dats_to_load_ == 0) {
    FinishCombinating();
  }
}

void AdBlockCombinatorFiltersProvider::FinishCombinating() {
  CHECK_EQ(dats_to_load_, 0);
  CHECK(cb_);
  if (combined_list_.size() == 0) {
    // Small workaround for code in
    // AdBlockService::SourceProviderObserver::OnResourcesLoaded that encodes a
    // state using an entirely empty DAT.
    //
    // This is only relevant in tests, since the default engine will generally
    // not be empty.
    combined_list_.push_back('\n');
  }
  std::move(cb_).Run(false, combined_list_);
  combined_list_.clear();
}

bool AdBlockCombinatorFiltersProvider::Delete() && {
  return true;
}

}  // namespace brave_shields
