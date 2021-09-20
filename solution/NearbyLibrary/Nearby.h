#pragma once

#include "Nearby.g.h"

#include "core/core.h"

namespace winrt::NearbyLibrary::implementation {
struct Nearby : NearbyT<Nearby> {
  Nearby();

  event_token ConnectionInitiated(
      Windows::Foundation::TypedEventHandler<
          NearbyLibrary::Nearby, ConnectionInitiatedEventArgs> const& handler) {
    return connection_initiated_event_.add(handler);
  }

  void ConnectionInitiated(event_token const& token) noexcept {
    connection_initiated_event_.remove(token);
  }

  event_token ConnectionAccepted(
      Windows::Foundation::TypedEventHandler<
          NearbyLibrary::Nearby, ConnectionAcceptedEventArgs> const& handler) {
    return connection_accepted_event_.add(handler);
  }
  void ConnectionAccepted(event_token const& token) noexcept {
    connection_accepted_event_.remove(token);
  }

  event_token ConnectionRejected(
      Windows::Foundation::TypedEventHandler<
          NearbyLibrary::Nearby, ConnectionRejectedEventArgs> const& handler) {
    return connection_rejected_event_.add(handler);
  }
  void ConnectionRejected(event_token const& token) noexcept {
    connection_rejected_event_.remove(token);
  }

  event_token ConnectionDisconnected(
      Windows::Foundation::TypedEventHandler<
          NearbyLibrary::Nearby, ConnectionDisconnectedEventArgs> const&
          handler) {
    return connection_disconnected_event_.add(handler);
  }
  void ConnectionDisconnected(event_token const& token) noexcept {
    connection_disconnected_event_.remove(token);
  }

  Windows::Foundation::IAsyncOperation<Status> StartAdvertising(
      hstring serviceId, ConnectionOptions options, hstring endpointInfo);
  hstring GetLocalEndpointId();

 private:
  // clang-format off
  event<Windows::Foundation::TypedEventHandler<NearbyLibrary::Nearby, ConnectionInitiatedEventArgs>> connection_initiated_event_;
  event<Windows::Foundation::TypedEventHandler<NearbyLibrary::Nearby, ConnectionAcceptedEventArgs>> connection_accepted_event_;
  event<Windows::Foundation::TypedEventHandler<NearbyLibrary::Nearby, ConnectionRejectedEventArgs>> connection_rejected_event_;
  event<Windows::Foundation::TypedEventHandler<NearbyLibrary::Nearby, ConnectionDisconnectedEventArgs>> connection_disconnected_event_;
  location::nearby::connections::ServiceControllerRouter router_;
  location::nearby::connections::Core core_;
  location::nearby::connections::ConnectionListener connection_listener_;
  // clang-format on
};
}  // namespace winrt::NearbyLibrary::implementation

namespace winrt::NearbyLibrary::factory_implementation {
struct Nearby : NearbyT<Nearby, implementation::Nearby> {};
}  // namespace winrt::NearbyLibrary::factory_implementation
