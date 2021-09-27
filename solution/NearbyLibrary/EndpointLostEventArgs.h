#pragma once

#include "EndpointLostEventArgs.g.h"

namespace winrt::NearbyLibrary::implementation {
struct EndpointLostEventArgs : EndpointLostEventArgsT<EndpointLostEventArgs> {
  EndpointLostEventArgs() = default;

  EndpointLostEventArgs(hstring const& endpointId) : endpoint_id_(endpointId) {}
  hstring EndpointId() { return endpoint_id_; }
  void EndpointId(hstring const& value) { endpoint_id_ = value; }

 private:
  hstring endpoint_id_;
};
}  // namespace winrt::NearbyLibrary::implementation

namespace winrt::NearbyLibrary::factory_implementation {
struct EndpointLostEventArgs
    : EndpointLostEventArgsT<EndpointLostEventArgs,
                             implementation::EndpointLostEventArgs> {};
}  // namespace winrt::NearbyLibrary::factory_implementation
