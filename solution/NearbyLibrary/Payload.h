#pragma once

#include "Payload.g.h"
#include "core/payload.h"

namespace winrt::NearbyLibrary::implementation {
struct Payload : PayloadT<Payload> {
  Payload() = default;

  Payload(Windows::Storage::Streams::IBuffer const& content)
      : content_(content) {}
  Payload(int64_t id, Windows::Storage::Streams::IBuffer const& content)
      : content_(content), id_(id) {}
  int64_t Id() { return id_; }
  Windows::Storage::Streams::IBuffer Content() { return content_; }
  int64_t Offset() { return offset_; }
  void Offset(int64_t value) { offset_ = value; }

 private:
  Windows::Storage::Streams::IBuffer content_{nullptr};
  int64_t id_{location::nearby::connections::Payload::GenerateId()};
  int64_t offset_;
};
}  // namespace winrt::NearbyLibrary::implementation

namespace winrt::NearbyLibrary::factory_implementation {
struct Payload : PayloadT<Payload, implementation::Payload> {};
}  // namespace winrt::NearbyLibrary::factory_implementation