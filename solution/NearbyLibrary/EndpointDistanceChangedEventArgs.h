#pragma once

#include "EndpointDistanceChangedEventArgs.g.h"

namespace winrt::NearbyLibrary::implementation {
struct EndpointDistanceChangedEventArgs
    : EndpointDistanceChangedEventArgsT<EndpointDistanceChangedEventArgs> {
  EndpointDistanceChangedEventArgs() = default;

  EndpointDistanceChangedEventArgs(DistanceInfo const& info,
                                   hstring const& endpointId)
      : info_(info), endpoint_id_(endpointId) {}
  DistanceInfo Info() { return info_; }
  void Info(DistanceInfo const& value) { info_ = value; }
  hstring EndpointId() { return endpoint_id_; }
  void EndpointId(hstring const& value) { endpoint_id_ = value; }

 private:
  DistanceInfo info_;
  hstring endpoint_id_;
};
}  // namespace winrt::NearbyLibrary::implementation

namespace winrt::NearbyLibrary::factory_implementation {
struct EndpointDistanceChangedEventArgs
    : EndpointDistanceChangedEventArgsT<
          EndpointDistanceChangedEventArgs,
          implementation::EndpointDistanceChangedEventArgs> {};
}  // namespace winrt::NearbyLibrary::factory_implementation
