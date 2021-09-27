#pragma once

#include "EndpointFoundEventArgs.g.h"

namespace winrt::NearbyLibrary::implementation {
struct EndpointFoundEventArgs
    : EndpointFoundEventArgsT<EndpointFoundEventArgs> {
  EndpointFoundEventArgs() = default;

  EndpointFoundEventArgs(hstring const& endpointId, hstring const& endpointInfo,
                         hstring const& serviceId)
      : endpoint_id_(endpointId),
        endpoint_info_(endpointInfo),
        service_id_(serviceId) {}
  hstring EndpointInfo() { return endpoint_info_; }
  void EndpointInfo(hstring const& value) { endpoint_info_ = value; }
  hstring ServiceId() { return service_id_; }
  void ServiceId(hstring const& value) { service_id_ = value; }
  hstring EndpointId() { return endpoint_id_; }
  void EndpointId(hstring const& value) { endpoint_id_ = value; }

 private:
  hstring endpoint_id_;
  hstring endpoint_info_;
  hstring service_id_;
};
}  // namespace winrt::NearbyLibrary::implementation

namespace winrt::NearbyLibrary::factory_implementation {
struct EndpointFoundEventArgs
    : EndpointFoundEventArgsT<EndpointFoundEventArgs,
                              implementation::EndpointFoundEventArgs> {};
}  // namespace winrt::NearbyLibrary::factory_implementation
