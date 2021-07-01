#include "pch.h"
#include "Nearby.h"
#include "Nearby.g.cpp"

using namespace location::nearby::connections;

namespace {
Strategy convert(const winrt::NearbyLibrary::Strategy& arg) {
  switch (arg) {
    case winrt::NearbyLibrary::Strategy::None:
      return Strategy::kNone;
    case winrt::NearbyLibrary::Strategy::P2pCluster:
      return Strategy::kP2pCluster;
    case winrt::NearbyLibrary::Strategy::P2pPointToPoint:
      return Strategy::kP2pPointToPoint;
    case winrt::NearbyLibrary::Strategy::P2pStar:
      return Strategy::kP2pStar;
    default:
      return Strategy::kNone;
  }
}

BooleanMediumSelector convert(const winrt::NearbyLibrary::MediumSelector& arg) {
  return {arg.Bluetooth(), arg.Ble(), arg.WebRtc(), arg.WifiLan()};
}

ConnectionOptions convert(const winrt::NearbyLibrary::ConnectionOptions& arg) {
  return {convert(arg.Strategy()),
          convert(arg.Allowed()),
          arg.AutoUpgradeBandwidth(),
          arg.EnforceTopologyConstraints(),
          arg.LowPower(),
          arg.EnableBluetoothListening(),
          arg.EnableWebrtcListening(),
          arg.IsOutOfBandConnection(),
          location::nearby::ByteArray{
              winrt::to_string(arg.RemoteBluetoothMacAddress())},
          winrt::to_string(arg.FastAdvertisementServiceUuid()),
          arg.KeepAliveIntervalMillis(),
          arg.KeepAliveTimeoutMillis()};
}

winrt::NearbyLibrary::ConnectionResponseInfo to_winrt(
    const ConnectionResponseInfo& arg) {
  auto response = winrt::NearbyLibrary::ConnectionResponseInfo{};
  auto remote_endpoint_info = std::string(arg.remote_endpoint_info);
  response.RemoteEndpointInfo(winrt::to_hstring(remote_endpoint_info));
  response.AuthenticationToken(winrt::to_hstring(arg.authentication_token));
  response.IsIncomingConnection(arg.is_incoming_connection);
  response.IsConnectionVerified(arg.is_connection_verified);
  if (arg.raw_authentication_token.size()) {
    auto copy = arg.raw_authentication_token;
    response.AuthenticationDigits(winrt::to_hstring(
        location::nearby::ByteUtils::ToFourDigitString(copy)));
  }

  return response;
}

winrt::NearbyLibrary::Status to_winrt(const Status& status) {
  switch (status.value) {
    case Status::kSuccess:
      return winrt::NearbyLibrary::Status{
          winrt::NearbyLibrary::StatusState::Success};
    case Status::kError:
      return winrt::NearbyLibrary::Status{
          winrt::NearbyLibrary::StatusState::Error};
    case Status::kOutOfOrderApiCall:
      return winrt::NearbyLibrary::Status{
          winrt::NearbyLibrary::StatusState::OutOfOrderApiCall};
    case Status::kAlreadyHaveActiveStrategy:
      return winrt::NearbyLibrary::Status{
          winrt::NearbyLibrary::StatusState::AlreadyHaveActiveStrategy};
    case Status::kAlreadyAdvertising:
      return winrt::NearbyLibrary::Status{
          winrt::NearbyLibrary::StatusState::AlreadyAdvertising};
    case Status::kAlreadyDiscovering:
      return winrt::NearbyLibrary::Status{
          winrt::NearbyLibrary::StatusState::AlreadyDiscovering};
    case Status::kEndpointIoError:
      return winrt::NearbyLibrary::Status{
          winrt::NearbyLibrary::StatusState::EndpointIoError};
    case Status::kEndpointUnknown:
      return winrt::NearbyLibrary::Status{
          winrt::NearbyLibrary::StatusState::EndpointUnknown};
    case Status::kConnectionRejected:
      return winrt::NearbyLibrary::Status{
          winrt::NearbyLibrary::StatusState::ConnectionRejected};
    case Status::kAlreadyConnectedToEndpoint:
      return winrt::NearbyLibrary::Status{
          winrt::NearbyLibrary::StatusState::AlreadyConnectedToEndpoint};
    case Status::kNotConnectedToEndpoint:
      return winrt::NearbyLibrary::Status{
          winrt::NearbyLibrary::StatusState::NotConnectedToEndpoint};
    case Status::kBluetoothError:
      return winrt::NearbyLibrary::Status{
          winrt::NearbyLibrary::StatusState::BluetoothError};
    case Status::kBleError:
      return winrt::NearbyLibrary::Status{
          winrt::NearbyLibrary::StatusState::BleError};
    case Status::kWifiLanError:
      return winrt::NearbyLibrary::Status{
          winrt::NearbyLibrary::StatusState::WifiLanError};
    case Status::kPayloadUnknown:
      return winrt::NearbyLibrary::Status{
          winrt::NearbyLibrary::StatusState::PayloadUnknown};
    default:
      return winrt::NearbyLibrary::Status{
          winrt::NearbyLibrary::StatusState::Error};
  }
}
}  // namespace

namespace winrt::NearbyLibrary::implementation {
Nearby::Nearby() {
  connection_listener_ = ConnectionListener{
      [this](
          const std::string& endpoint_id,
          const location::nearby::connections::ConnectionResponseInfo& info) {
        connection_initiated_event_(
            *this, ConnectionInitiatedEventArgs{to_winrt(info),
                                                to_hstring(endpoint_id)});
      }};
}

Windows::Foundation::IAsyncOperation<Status> Nearby::StartAdvertising(
    hstring serviceId, ConnectionOptions options, hstring endpointInfo) {
  struct awaitable : std::suspend_always {
    Core& nearby_core;
    hstring service_id;
    ConnectionOptions options;
    hstring endpoint_info;
    ConnectionListener listener;
    Status result;

    awaitable(Core& nearby_core, hstring service_id, ConnectionOptions options,
              hstring endpoint_info, const ConnectionListener& listener)
        : nearby_core(nearby_core),
          service_id(service_id),
          options(options),
          endpoint_info(endpoint_info),
          listener(listener) {}

    bool await_ready() { return false; }

    void await_suspend(std::coroutine_handle<> handle) {
      nearby_core.StartAdvertising(
          to_string(service_id), convert(options),
          ConnectionRequestInfo{
              location::nearby::ByteArray{to_string(endpoint_info)},
              std::move(listener)},
          ResultCallback{[this, handle](::Status status) {
            result = to_winrt(status);
            handle.resume();
          }});
    }

    Status await_resume() { return result; }
  };

  auto result = co_await awaitable{core_, serviceId, options, endpointInfo,
                                   connection_listener_};
  co_return result;
}

hstring Nearby::GetLocalEndpointId() {
  return winrt::to_hstring(core_.GetLocalEndpointId());
}
}  // namespace winrt::NearbyLibrary::implementation
