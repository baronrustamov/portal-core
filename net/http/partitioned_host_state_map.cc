/* Copyright 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/net/http/partitioned_host_state_map.h"

#include <utility>

#include "base/ranges/algorithm.h"
#include "crypto/sha2.h"
#include "net/base/network_isolation_key.h"

namespace net {

PartitionedHostStateMapBase::PartitionedHostStateMapBase() = default;
PartitionedHostStateMapBase::~PartitionedHostStateMapBase() = default;

base::AutoReset<absl::optional<PartitionedHostStateMapBase::HashedHost>>
PartitionedHostStateMapBase::SetScopedPartitionHash(
    absl::optional<HashedHost> partition_hash) {
  return base::AutoReset<absl::optional<HashedHost>>(&partition_hash_,
                                                     std::move(partition_hash));
}

bool PartitionedHostStateMapBase::HasPartitionHash() const {
  return partition_hash_.has_value();
}

bool PartitionedHostStateMapBase::IsPartitionHashValid() const {
  return partition_hash_ && !partition_hash_->empty();
}

PartitionedHostStateMapBase::HashedHost
PartitionedHostStateMapBase::GetKeyWithPartitionHash(
    const HashedHost& k) const {
  CHECK(IsPartitionHashValid());
  if (k == *partition_hash_) {
    return k;
  }

  std::array<uint8_t, crypto::kSHA256Length> result;
  base::ranges::copy(GetHalfKey(k), result.begin());
  base::ranges::copy(GetHalfKey(*partition_hash_),
                     std::next(result.begin(), crypto::kSHA256Length / 2));
  return result;
}

base::span<uint8_t> PartitionedHostStateMapBase::GetHalfKey(HashedHost k) {
  return base::make_span(k.data(), crypto::kSHA256Length / 2);
}

}  // namespace net
