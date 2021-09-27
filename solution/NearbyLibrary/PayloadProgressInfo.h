#pragma once

#include "PayloadProgressInfo.g.h"

namespace winrt::NearbyLibrary::implementation {
struct PayloadProgressInfo : PayloadProgressInfoT<PayloadProgressInfo> {
  PayloadProgressInfo() = default;

  int64_t PayloadId() { return payload_id_; }
  void PayloadId(int64_t value) { payload_id_ = value; }
  int64_t TotalBytes() { return total_bytes_; }
  void TotalBytes(int64_t value) { total_bytes_ = value; }
  int64_t BytesTransferred() { return bytes_transferred_; }
  void BytesTransferred(int64_t value) { bytes_transferred_ = value; }
  PayloadProgressStatus Status() { return status_; }
  void Status(PayloadProgressStatus const& value) { status_ = value; }

 private:
  int64_t payload_id_;
  int64_t total_bytes_;
  int64_t bytes_transferred_;
  PayloadProgressStatus status_;
};
}  // namespace winrt::NearbyLibrary::implementation

namespace winrt::NearbyLibrary::factory_implementation {
struct PayloadProgressInfo
    : PayloadProgressInfoT<PayloadProgressInfo,
                           implementation::PayloadProgressInfo> {};
}  // namespace winrt::NearbyLibrary::factory_implementation
