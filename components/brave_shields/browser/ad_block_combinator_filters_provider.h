/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_SHIELDS_BROWSER_AD_BLOCK_COMBINATOR_FILTERS_PROVIDER_H_
#define BRAVE_COMPONENTS_BRAVE_SHIELDS_BROWSER_AD_BLOCK_COMBINATOR_FILTERS_PROVIDER_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/observer_list.h"
#include "brave/components/brave_component_updater/browser/dat_file_util.h"
#include "brave/components/brave_shields/browser/ad_block_filters_provider.h"

using brave_component_updater::DATFileDataBuffer;

namespace brave_shields {

class AdBlockCombinatorFiltersProvider
    : public AdBlockFiltersProvider,
      public AdBlockFiltersProvider::Observer {
 public:
  AdBlockCombinatorFiltersProvider();
  ~AdBlockCombinatorFiltersProvider() override;
  AdBlockCombinatorFiltersProvider(const AdBlockCombinatorFiltersProvider&) =
      delete;
  AdBlockCombinatorFiltersProvider& operator=(
      const AdBlockCombinatorFiltersProvider&) = delete;

  void LoadDATBuffer(
      base::OnceCallback<void(bool deserialize,
                              const DATFileDataBuffer& dat_buf)>) override;

  bool Delete() && override;

  // AdBlockFiltersProvider::Observer
  void OnNewSourceAvailable(
      base::WeakPtr<AdBlockFiltersProvider> from) override;

  void CombinateProvider(AdBlockFiltersProvider* provider);
  void UncombinateProvider(AdBlockFiltersProvider* provider);

 private:
  int dats_to_load_ = 0;
  int request_nonce_ = 0;
  DATFileDataBuffer combined_list_;
  base::OnceCallback<void(bool, const DATFileDataBuffer&)> cb_;
  void FinishCombinating();
  void OnDATLoaded(int request_nonce,
                   bool deserialize,
                   const DATFileDataBuffer& dat_buf);
  std::vector<AdBlockFiltersProvider*> combinated_providers_;

  base::WeakPtrFactory<AdBlockCombinatorFiltersProvider> weak_factory_{this};
};

}  // namespace brave_shields

#endif  // BRAVE_COMPONENTS_BRAVE_SHIELDS_BROWSER_AD_BLOCK_COMBINATOR_FILTERS_PROVIDER_H_
