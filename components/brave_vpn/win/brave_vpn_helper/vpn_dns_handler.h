// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef BRAVE_COMPONENTS_BRAVE_VPN_WIN_BRAVE_VPN_HELPER_VPN_DNS_HANDLER_H_
#define BRAVE_COMPONENTS_BRAVE_VPN_WIN_BRAVE_VPN_HELPER_VPN_DNS_HANDLER_H_

#include <windows.h>
#include <string>

#include "base/memory/raw_ptr.h"
#include "base/synchronization/waitable_event.h"
#include "base/task/sequenced_task_runner.h"
#include "base/timer/timer.h"
#include "base/win/object_watcher.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace brave_vpn {

namespace internal {
enum class CheckConnectionResult;
}  // namespace internal

class MockVpnDnsHandler;

class VpnDnsHandler : public base::win::ObjectWatcher::Delegate {
 public:
  VpnDnsHandler();
  ~VpnDnsHandler() override;

  bool SetFilters(const std::wstring& connection_name);
  bool RemoveFilters(const std::wstring& connection_name);
  bool IsActive() const;
  void SetConnectionResultForTesting(internal::CheckConnectionResult result);
  void SetCloseEngineResultForTesting(bool value);
  void SetPlatformFiltersResultForTesting(bool value);
  void CheckConnectionStatus();

 protected:
  // base::win::ObjectWatcher::Delegate overrides:
  void OnObjectSignaled(HANDLE object) override;
  void StartVPNConnectionChangeMonitoring();
  internal::CheckConnectionResult GetVpnEntryState();
  void OnCheckConnection(internal::CheckConnectionResult result);
  bool CloseEngineSession();
  void Exit();
  virtual void SignalExit() = 0;

 private:
  friend class MockVpnDnsHandler;
  FRIEND_TEST_ALL_PREFIXES(VpnDnsHandlerTest,
                           ConnectingSuccessFiltersInstalled);

  bool SetupPlatformFilters(HANDLE engine_handle, const std::string& name);
  void CloseWatchers();
  virtual void SubscribeForRasNotifications(HANDLE event_handle);

  absl::optional<internal::CheckConnectionResult>
      connection_result_for_testing_;
  absl::optional<bool> platform_filters_result_for_testing_;
  absl::optional<bool> close_engine_result_for_testing_;

  HANDLE engine_ = nullptr;
  HANDLE event_handle_for_vpn_ = nullptr;
  base::win::ObjectWatcher connected_disconnected_event_watcher_;
  base::RepeatingTimer periodic_timer_;
  base::WeakPtrFactory<VpnDnsHandler> weak_factory_{this};
};

}  // namespace brave_vpn

#endif  // BRAVE_COMPONENTS_BRAVE_VPN_WIN_BRAVE_VPN_HELPER_VPN_DNS_HANDLER_H_
