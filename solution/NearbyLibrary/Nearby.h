#pragma once

#include "Nearby.g.h"

#include "core/core.h"

namespace winrt::NearbyLibrary::implementation {
struct Nearby : NearbyT<Nearby> {
  Nearby();

#pragma region Advertising Events
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
#pragma endregion

#pragma region Discovery Events
  event_token EndpointFound(
      Windows::Foundation::TypedEventHandler<
          NearbyLibrary::Nearby, EndpointFoundEventArgs> const& handler) {
    return endpoint_found_event_.add(handler);
  }
  void EndpointFound(event_token const& token) noexcept {
    endpoint_found_event_.remove(token);
  }

  event_token EndpointLost(
      Windows::Foundation::TypedEventHandler<
          NearbyLibrary::Nearby, EndpointLostEventArgs> const& handler) {
    return endpoint_lost_event_.add(handler);
  }
  void EndpointLost(event_token const& token) noexcept {
    endpoint_lost_event_.remove(token);
  }

  event_token EndpointDistanceChanged(
      Windows::Foundation::TypedEventHandler<
          NearbyLibrary::Nearby, EndpointDistanceChangedEventArgs> const&
          handler) {
    return endpoint_distance_changed_event_.add(handler);
  }
  void EndpointDistanceChanged(event_token const& token) noexcept {
    endpoint_distance_changed_event_.remove(token);
  }
#pragma endregion

  // clang-format off
  Windows::Foundation::IAsyncOperation<Status> StartAdvertisingAsync(hstring serviceId, ConnectionOptions options, hstring endpointInfo);
  Windows::Foundation::IAsyncOperation<Status> StopAdvertisingAsync();
  Windows::Foundation::IAsyncOperation<Status> StartDiscoveryAsync(hstring serviceId, ConnectionOptions options);
  Windows::Foundation::IAsyncOperation<Status> StopDiscoveryAsync();
  Windows::Foundation::IAsyncOperation<Status> InjectEndpointAsync(hstring serviceId, winrt::NearbyLibrary::OutOfBandConnectionMetadata metadata);
  Windows::Foundation::IAsyncOperation<Status> RequestConnectionAsync(hstring endpointId, hstring endpointInfo, ConnectionOptions options);
  hstring GetLocalEndpointId();
  // clang-format on

 private:
  // clang-format off
    event<Windows::Foundation::TypedEventHandler<NearbyLibrary::Nearby, ConnectionInitiatedEventArgs>> connection_initiated_event_;
    event<Windows::Foundation::TypedEventHandler<NearbyLibrary::Nearby, ConnectionAcceptedEventArgs>> connection_accepted_event_;
    event<Windows::Foundation::TypedEventHandler<NearbyLibrary::Nearby, ConnectionRejectedEventArgs>> connection_rejected_event_;
    event<Windows::Foundation::TypedEventHandler<NearbyLibrary::Nearby, ConnectionDisconnectedEventArgs>> connection_disconnected_event_;
    event<Windows::Foundation::TypedEventHandler<NearbyLibrary::Nearby, EndpointFoundEventArgs>> endpoint_found_event_;
    event<Windows::Foundation::TypedEventHandler<NearbyLibrary::Nearby, EndpointLostEventArgs>> endpoint_lost_event_;
    event<Windows::Foundation::TypedEventHandler<NearbyLibrary::Nearby, EndpointDistanceChangedEventArgs>> endpoint_distance_changed_event_;
    location::nearby::connections::ServiceControllerRouter router_;
    location::nearby::connections::Core core_;
    location::nearby::connections::ConnectionListener connection_listener_;
  // clang-format on
};
}  // namespace winrt::NearbyLibrary::implementation

namespace winrt::NearbyLibrary::factory_implementation {
struct Nearby : NearbyT<Nearby, implementation::Nearby> {};
}  // namespace winrt::NearbyLibrary::factory_implementation
