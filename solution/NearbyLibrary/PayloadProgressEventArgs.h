#pragma once

#include "PayloadProgressEventArgs.g.h"

namespace winrt::NearbyLibrary::implementation {
struct PayloadProgressEventArgs
    : PayloadProgressEventArgsT<PayloadProgressEventArgs> {
  PayloadProgressEventArgs() = default;

  PayloadProgressEventArgs(hstring const& endpointId,
                           PayloadProgressInfo const& info)
      : endpoint_id_(endpointId), info_(info) {}
  PayloadProgressInfo Info() { return info_; }
  void Info(PayloadProgressInfo const& value) { info_ = value; }
  hstring EndpointId() { return endpoint_id_; }
  void EndpointId(hstring const& value) { endpoint_id_ = value; }

 private:
  hstring endpoint_id_;
  struct PayloadProgressInfo info_;
};
}  // namespace winrt::NearbyLibrary::implementation

namespace winrt::NearbyLibrary::factory_implementation {
struct PayloadProgressEventArgs
    : PayloadProgressEventArgsT<PayloadProgressEventArgs,
                                implementation::PayloadProgressEventArgs> {};
}  // namespace winrt::NearbyLibrary::factory_implementation
