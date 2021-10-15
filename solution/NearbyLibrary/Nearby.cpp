#include "pch.h"
#include "Nearby.h"
#include "Nearby.g.cpp"

#include "winrt/Windows.Storage.Streams.h"

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
  return {
      convert(arg.Strategy()),
      convert(arg.Allowed()),
      arg.AutoUpgradeBandwidth(),
      arg.EnforceTopologyConstraints(),
      arg.LowPower(),
      arg.EnableBluetoothListening(),
      arg.EnableWebrtcListening(),
      arg.IsOutOfBandConnection(),
      location::nearby::ByteArray{to_string(arg.RemoteBluetoothMacAddress())},
      to_string(arg.FastAdvertisementServiceUuid()),
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

winrt::NearbyLibrary::PayloadProgressInfo to_winrt(
    const PayloadProgressInfo& info) {
  winrt::NearbyLibrary::PayloadProgressInfo value;
  value.PayloadId(info.payload_id);
  value.Status(
      static_cast<winrt::NearbyLibrary::PayloadProgressStatus>(info.status));
  value.TotalBytes(info.total_bytes);
  value.BytesTransferred(info.bytes_transferred);
  return value;
}

winrt::NearbyLibrary::Payload to_winrt(const Payload& value) {
  auto& bytes = value.AsBytes();
  winrt::array_view arr_view = {reinterpret_cast<const uint8_t*>(bytes.data()),
                                static_cast<uint32_t>(bytes.size())};
  winrt::Windows::Storage::Streams::DataWriter data_writer;
  data_writer.WriteBytes(arr_view);
  return winrt::NearbyLibrary::Payload{value.GetId(),
                                       data_writer.DetachBuffer()};
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
Nearby::Nearby()
    : core_(Core(&router_)),
      connection_listener_(MakeConnectionListener()),
      discovery_listener_(MakeDiscoveryListener()),
      payload_listener_(MakePayloadListener()) {}

Windows::Foundation::IAsyncOperation<Status> Nearby::StartAdvertisingAsync(
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

Windows::Foundation::IAsyncOperation<Status> Nearby::StopAdvertisingAsync() {
  struct awaitable : std::suspend_always {
    Core& nearby_core;
    Status result;

    awaitable(Core& nearby_core) : nearby_core(nearby_core) {}

    bool await_ready() { return false; }

    void await_suspend(std::coroutine_handle<> handle) {
      nearby_core.StopAdvertising(
          ResultCallback{[this, handle](::Status status) {
            result = to_winrt(status);
            handle.resume();
          }});
    }

    Status await_resume() { return result; }
  };

  auto result = co_await awaitable{core_};
  co_return result;
}

Windows::Foundation::IAsyncOperation<Status> Nearby::StartDiscoveryAsync(
    hstring serviceId, ConnectionOptions options) {
  struct awaitable : std::suspend_always {
    Core& nearby_core;
    hstring service_id;
    ConnectionOptions options;
    DiscoveryListener listener;
    Status result;

    awaitable(Core& nearby_core, hstring service_id, ConnectionOptions options,
              DiscoveryListener listener)
        : nearby_core(nearby_core),
          service_id(service_id),
          options(options),
          listener(listener) {}

    bool await_ready() { return false; }

    void await_suspend(std::coroutine_handle<> handle) {
      nearby_core.StartDiscovery(
          to_string(service_id), convert(options), std::move(listener),
          ResultCallback{[this, handle](::Status status) {
            result = to_winrt(status);
            handle.resume();
          }});
    }

    Status await_resume() { return result; }
  };

  auto result =
      co_await awaitable{core_, serviceId, options, discovery_listener_};
  co_return result;
}

Windows::Foundation::IAsyncOperation<Status> Nearby::StopDiscoveryAsync() {
  struct awaitable : std::suspend_always {
    Core& nearby_core;
    Status result;

    awaitable(Core& nearby_core) : nearby_core(nearby_core) {}

    bool await_ready() { return false; }

    void await_suspend(std::coroutine_handle<> handle) {
      nearby_core.StopDiscovery(ResultCallback{[this, handle](::Status status) {
        result = to_winrt(status);
        handle.resume();
      }});
    }

    Status await_resume() { return result; }
  };

  auto result = co_await awaitable{core_};
  co_return result;
}

Windows::Foundation::IAsyncOperation<Status> Nearby::InjectEndpointAsync(
    hstring serviceId, OutOfBandConnectionMetadata metadata) {
  throw hresult_not_implemented();
}

Windows::Foundation::IAsyncOperation<Status> Nearby::RequestConnectionAsync(
    hstring endpointId, hstring endpointInfo, ConnectionOptions options) {
  throw hresult_not_implemented();
}

Windows::Foundation::IAsyncOperation<Status> Nearby::AcceptConnectionAsync(
    hstring endpointId) {
  throw hresult_not_implemented();
}

Windows::Foundation::IAsyncOperation<Status> Nearby::RejectConnectionAsync(
    hstring endpointId) {
  throw hresult_not_implemented();
}

Windows::Foundation::IAsyncOperation<Status> Nearby::SendPayloadAsync(
    array_view<hstring const> endpointIds, Payload payload) {
  throw hresult_not_implemented();
}

Windows::Foundation::IAsyncOperation<Status> Nearby::CancelPayloadAsync(
    int64_t payloadId) {
  throw hresult_not_implemented();
}

Windows::Foundation::IAsyncOperation<Status>
Nearby::DisconnectFromEndpointAsync(hstring endpointId) {
  throw hresult_not_implemented();
}

Windows::Foundation::IAsyncOperation<Status> Nearby::StopAllEndpointsAsync() {
  throw hresult_not_implemented();
}

Windows::Foundation::IAsyncOperation<Status>
Nearby::InitiateBandwidthUpgradeAsync(hstring endpointId) {
  throw hresult_not_implemented();
}

hstring Nearby::GetLocalEndpointId() {
  return to_hstring(core_.GetLocalEndpointId());
}

ConnectionListener Nearby::MakeConnectionListener() {
  return {
      .initiated_cb =
          [this](const std::string& endpoint_id,
                 const location::nearby::connections::ConnectionResponseInfo&
                     info) {
            connection_initiated_event_(
                *this, ConnectionInitiatedEventArgs{to_winrt(info),
                                                    to_hstring(endpoint_id)});
          },
      .accepted_cb =
          [this](const std::string& endpoint_id) {
            connection_accepted_event_(
                *this, ConnectionAcceptedEventArgs(to_hstring(endpoint_id)));
          },
      .rejected_cb =
          [this](const std::string& endpoint_id,
                 location::nearby::connections::Status status) {
            connection_rejected_event_(
                *this, ConnectionRejectedEventArgs(to_winrt(status),
                                                   to_hstring(endpoint_id)));
          },
      .disconnected_cb =
          [this](const std::string& endpoint_id) {
            connection_disconnected_event_(
                *this,
                ConnectionDisconnectedEventArgs(to_hstring(endpoint_id)));
          },
      .bandwidth_changed_cb =
          [this](const std::string& endpoint_id,
                 location::nearby::connections::Medium medium) {
            connection_bandwidth_changed_event_(
                *this,
                ConnectionBandwidthChangedEventArgs(static_cast<Medium>(medium),
                                                    to_hstring(endpoint_id)));
          }};
}

DiscoveryListener Nearby::MakeDiscoveryListener() {
  return {
      .endpoint_found_cb =
          [this](const std::string& endpoint_id,
                 const location::nearby::ByteArray& endpoint_info,
                 const std::string& service_id) {
            endpoint_found_event_(
                *this, EndpointFoundEventArgs(
                           to_hstring(endpoint_id),
                           to_hstring(static_cast<std::string>(endpoint_info)),
                           to_hstring(service_id)));
          },
      .endpoint_lost_cb =
          [this](const std::string& endpoint_id) {
            endpoint_lost_event_(
                *this, EndpointLostEventArgs(to_hstring(endpoint_id)));
          },
      .endpoint_distance_changed_cb =
          [this](const std::string& endpoint_id,
                 location::nearby::connections::DistanceInfo info) {
            endpoint_distance_changed_event_(
                *this,
                EndpointDistanceChangedEventArgs(
                    static_cast<DistanceInfo>(info), to_hstring(endpoint_id)));
          }};
}

PayloadListener Nearby::MakePayloadListener() {
  return {
      .payload_cb =
          [this](const std::string& endpoint_id,
                 location::nearby::connections::Payload payload) {
            payload_received_event_(
                *this,
                PayloadEventArgs(to_hstring(endpoint_id), to_winrt(payload)));
          },
      .payload_progress_cb =
          [this](
              const std::string& endpoint_id,
              const location::nearby::connections::PayloadProgressInfo& info) {
            payload_progress_changed_event_(
                *this, PayloadProgressEventArgs(to_hstring(endpoint_id),
                                                to_winrt(info)));
          }};
}
}  // namespace winrt::NearbyLibrary::implementation
