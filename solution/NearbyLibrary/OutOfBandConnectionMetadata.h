#pragma once

#include "OutOfBandConnectionMetadata.g.h"

namespace winrt::NearbyLibrary::implementation {
struct OutOfBandConnectionMetadata
    : OutOfBandConnectionMetadataT<OutOfBandConnectionMetadata> {
  OutOfBandConnectionMetadata() = default;

  Medium Medium() { return medium_; }
  void Medium(NearbyLibrary::Medium const& value) { medium_ = value; }
  hstring EndpointId() { return endpoint_id_; }
  void EndpointId(hstring const& value) { endpoint_id_ = value; }
  hstring EndpointInfo() { return endpoint_info_; }
  void EndpointInfo(hstring const& value) { endpoint_info_ = value; }
  hstring RemoteBluetoothMacAddress() { return remote_bluetooth_mac_address_; }
  void RemoteBluetoothMacAddress(hstring const& value) {
    remote_bluetooth_mac_address_ = value;
  }

 private:
  enum Medium medium_;
  hstring endpoint_id_;
  hstring endpoint_info_;
  hstring remote_bluetooth_mac_address_;
};
}  // namespace winrt::NearbyLibrary::implementation

namespace winrt::NearbyLibrary::factory_implementation {
struct OutOfBandConnectionMetadata
    : OutOfBandConnectionMetadataT<
          OutOfBandConnectionMetadata,
          implementation::OutOfBandConnectionMetadata> {};
}  // namespace winrt::NearbyLibrary::factory_implementation
