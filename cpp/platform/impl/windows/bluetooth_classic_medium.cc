// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "pch.h"
#include "bluetooth_classic_medium.h"

#include "bluetooth_classic_device.h"
#include <winrt/Windows.Devices.Bluetooth.h>
#include <winrt/Windows.Devices.Enumeration.h>
#include <winrt/Windows.Foundation.Collections.h>

namespace location {
namespace nearby {
namespace windows {

BluetoothClassicMedium::BluetoothClassicMedium() {
  InitializeCriticalSection(&critical_section);

  // create watcher
  device_watcher_ = DeviceInformation::CreateWatcher(
      BLUETOOTH_SELECTOR, nullptr, DeviceInformationKind::AssociationEndpoint);

  // An app must subscribe to all of the added, removed, and updated events to
  // be notified when there are device additions, removals or updates. If an app
  // handles only the added event, it will not receive an update if a device is
  // added to the system after the initial device enumeration completes.
  // register event handlers before starting the watcher
  // https://docs.microsoft.com/en-us/uwp/api/windows.devices.enumeration.devicewatcher.added?view=winrt-20348
  device_watcher_.Added(winrt::auto_revoke,
                        {this, &BluetoothClassicMedium::DeviceWatcher_Added});

  // https://docs.microsoft.com/en-us/uwp/api/windows.devices.enumeration.devicewatcher.updated?view=winrt-20348
  device_watcher_.Updated(
      winrt::auto_revoke,
      {this, &BluetoothClassicMedium::DeviceWatcher_Updated});

  // https://docs.microsoft.com/en-us/uwp/api/windows.devices.enumeration.devicewatcher.removed?view=winrt-20348
  device_watcher_.Removed(
      winrt::auto_revoke,
      {this, &BluetoothClassicMedium::DeviceWatcher_Removed});
}

BluetoothClassicMedium::~BluetoothClassicMedium() {}

// TODO(b/184975123): replace with real implementation.
location::nearby::api::BluetoothDevice* GetRemoteDevice(
    const std::string& mac_address) {
  return nullptr;
}

bool BluetoothClassicMedium::StartDiscovery(
    BluetoothClassicMedium::DiscoveryCallback discovery_callback) {
  EnterCriticalSection(&critical_section);

  bool result = false;
  discovery_callback_ = discovery_callback;

  if (!IsWatcherStarted()) {
    result = StartScanning();
  }

  LeaveCriticalSection(&critical_section);

  return result;
}

bool BluetoothClassicMedium::StopDiscovery() {
  EnterCriticalSection(&critical_section);

  bool result = false;

  if (IsWatcherStarted()) {
    result = StopScanning();
  }

  LeaveCriticalSection(&critical_section);

  return result;
}

std::unique_ptr<location::nearby::api::BluetoothSocket>
BluetoothClassicMedium::ConnectToService(
    location::nearby::api::BluetoothDevice& remote_device,
    const std::string& service_uuid,
    location::nearby::CancellationFlag* cancellation_flag) {
  return nullptr;
}

// https://developer.android.com/reference/android/bluetooth/BluetoothAdapter.html#listenUsingInsecureRfcommWithServiceRecord
//
// service_uuid is the canonical textual representation
// (https://en.wikipedia.org/wiki/Universally_unique_identifier#Format) of a
// type 3 name-based
// (https://en.wikipedia.org/wiki/Universally_unique_identifier#Versions_3_and_5_(namespace_name-based))
// UUID.
//
//  Returns nullptr error.
// TODO(b/184975123): replace with real implementation.
std::unique_ptr<location::nearby::api::BluetoothServerSocket>
BluetoothClassicMedium::ListenForService(const std::string& service_name,
                                         const std::string& service_uuid) {
  return nullptr;
}

// TODO(b/184975123): replace with real implementation.
location::nearby::api::BluetoothDevice* BluetoothClassicMedium::GetRemoteDevice(
    const std::string& mac_address) {
  return nullptr;
}

bool BluetoothClassicMedium::StartScanning() {
  if (!IsWatcherStarted()) {
    // The Start method can only be called when the DeviceWatcher is in the
    // Created, Stopped or Aborted state.
    auto status = device_watcher_.Status();

    if (status == DeviceWatcherStatus::Created ||
        status == DeviceWatcherStatus::Stopped ||
        status == DeviceWatcherStatus::Aborted) {
      device_watcher_.Start();

      return true;
    }
  }

  return false;
}

bool BluetoothClassicMedium::StopScanning() {
  if (IsWatcherRunning()) {
    device_watcher_.Stop();
    return true;
  }

  return false;
}

winrt::fire_and_forget BluetoothClassicMedium::DeviceWatcher_Added(
    DeviceWatcher sender, DeviceInformation deviceInfo) {
  if (IsWatcherStarted()) {
    // https://docs.microsoft.com/en-us/uwp/api/windows.devices.bluetooth.bluetoothdevice?view=winrt-20348
    std::unique_ptr<winrt::Windows::Devices::Bluetooth::BluetoothDevice>
        windowsBluetoothDevice;

    // Create an iterator for the internal list
    std::map<winrt::hstring, std::unique_ptr<BluetoothDevice>>::const_iterator
        it = devices_by_id_.find(deviceInfo.Id());

    // Add to our internal list if necessary
    if (it == devices_by_id_.end()) {
      // Create a bluetooth device out of this id
      winrt::Windows::Devices::Bluetooth::BluetoothDevice::FromIdAsync(
          deviceInfo.Id())
          .Completed(
              [this, deviceInfo, wbd = std::move(windowsBluetoothDevice)](
                  auto&& async,
                  winrt::Windows::Foundation::AsyncStatus status) {
                EnterCriticalSection(&critical_section);

                std::unique_ptr<BluetoothDevice> bluetoothDevice =
                    std::make_unique<BluetoothDevice>(async.GetResults());

                devices_by_id_[deviceInfo.Id()] = std::move(bluetoothDevice);

                LeaveCriticalSection(&critical_section);

                if (discovery_callback_.device_discovered_cb != nullptr) {
                  discovery_callback_.device_discovered_cb(
                      *devices_by_id_[deviceInfo.Id()]);
                }
              });
    }
  }

  return winrt::fire_and_forget();
}

winrt::fire_and_forget BluetoothClassicMedium::DeviceWatcher_Updated(
    DeviceWatcher sender, DeviceInformationUpdate deviceInfoUpdate) {
  if (IsWatcherStarted()) {
    // Check for device name change
  }

  return winrt::fire_and_forget();
}

winrt::fire_and_forget BluetoothClassicMedium::DeviceWatcher_Removed(
    DeviceWatcher sender, DeviceInformationUpdate deviceInfo) {
  if (IsWatcherStarted()) {
    if (discovery_callback_.device_lost_cb != nullptr) {
      discovery_callback_.device_lost_cb(*devices_by_id_[deviceInfo.Id()]);
    }

    devices_by_id_.erase(deviceInfo.Id());
  }

  return winrt::fire_and_forget();
}

bool BluetoothClassicMedium::IsWatcherStarted() {
  if (device_watcher_ == nullptr) {
    return false;
  }

  DeviceWatcherStatus status = device_watcher_.Status();
  return (status == DeviceWatcherStatus::Started) ||
         (status == DeviceWatcherStatus::EnumerationCompleted);
}

bool BluetoothClassicMedium::IsWatcherRunning() {
  if (device_watcher_ == nullptr) {
    return false;
  }

  DeviceWatcherStatus status = device_watcher_.Status();
  return (status == DeviceWatcherStatus::Started) ||
         (status == DeviceWatcherStatus::EnumerationCompleted) ||
         (status == DeviceWatcherStatus::Stopping);
}

}  // namespace windows
}  // namespace nearby
}  // namespace location
