#pragma once

#include "PayloadEventArgs.g.h"

namespace winrt::NearbyLibrary::implementation {
struct PayloadEventArgs : PayloadEventArgsT<PayloadEventArgs> {
  PayloadEventArgs() = default;

  PayloadEventArgs(hstring const& endpointId, Payload const& payload)
      : endpoint_id_(endpointId), payload_(payload) {}
  Payload Payload() { return payload_; }
  void Payload(NearbyLibrary::Payload const& value) { payload_ = value; }
  hstring EndpointId() { return endpoint_id_; }
  void EndpointId(hstring const& value) { endpoint_id_ = value; }

 private:
  hstring endpoint_id_;
  struct Payload payload_;
};
}  // namespace winrt::NearbyLibrary::implementation

namespace winrt::NearbyLibrary::factory_implementation {
struct PayloadEventArgs
    : PayloadEventArgsT<PayloadEventArgs, implementation::PayloadEventArgs> {};
}  // namespace winrt::NearbyLibrary::factory_implementation
