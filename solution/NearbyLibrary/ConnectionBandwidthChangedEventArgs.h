#pragma once

#include "ConnectionBandwidthChangedEventArgs.g.h"

namespace winrt::NearbyLibrary::implementation {
struct ConnectionBandwidthChangedEventArgs
    : ConnectionBandwidthChangedEventArgsT<
          ConnectionBandwidthChangedEventArgs> {
  ConnectionBandwidthChangedEventArgs() = default;

  ConnectionBandwidthChangedEventArgs(Medium const& medium,
                                      hstring const& endpointId)
      : endpoint_id_(endpointId), medium_(medium) {}
  Medium Medium() { return medium_; }
  void Medium(NearbyLibrary::Medium const& value) { medium_ = value; }
  hstring EndpointId() { return endpoint_id_; }
  void EndpointId(hstring const& value) { endpoint_id_ = value; }

 private:
  hstring endpoint_id_;
  enum Medium medium_ { Medium::UnknownMedium };
};
}  // namespace winrt::NearbyLibrary::implementation
namespace winrt::NearbyLibrary::factory_implementation {
struct ConnectionBandwidthChangedEventArgs
    : ConnectionBandwidthChangedEventArgsT<
          ConnectionBandwidthChangedEventArgs,
          implementation::ConnectionBandwidthChangedEventArgs> {};
}  // namespace winrt::NearbyLibrary::factory_implementation
