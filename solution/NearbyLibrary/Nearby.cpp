#include "pch.h"
#include "Nearby.h"
#include "Nearby.g.cpp"

#include <ranges>

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
  BooleanMediumSelector selector;
  selector.bluetooth = arg.Bluetooth();
  selector.ble = arg.Ble();
  selector.web_rtc = arg.WebRtc();
  selector.wifi_lan = arg.WifiLan();
  return selector;
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

OutOfBandConnectionMetadata convert(
    const winrt::NearbyLibrary::OutOfBandConnectionMetadata& arg) {
  return {
      static_cast<Medium>(arg.Medium()), to_string(arg.EndpointId()),
      location::nearby::ByteArray{to_string(arg.EndpointInfo())},
      location::nearby::ByteArray{to_string(arg.RemoteBluetoothMacAddress())}};
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
    std::string service_id;
    ::ConnectionOptions options;
    ConnectionRequestInfo info;
    Status result;

    awaitable(Core& nearby_core, std::string&& service_id,
              ::ConnectionOptions&& options, ConnectionRequestInfo&& info)
        : nearby_core(nearby_core),
          service_id(std::move(service_id)),
          options(std::move(options)),
          info(std::move(info)) {}

    bool await_ready() { return false; }

    void await_suspend(std::coroutine_handle<> handle) {
      nearby_core.StartAdvertising(
          service_id, std::move(options), std::move(info),
          ResultCallback{[this, handle](::Status status) {
            result = to_winrt(status);
            handle.resume();
          }});
    }

    Status await_resume() { return std::move(result); }
  };

  auto result = co_await awaitable{
      core_, to_string(serviceId), convert(options),
      ConnectionRequestInfo{
          location::nearby::ByteArray{to_string(endpointInfo)},
          connection_listener_}};
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

    Status await_resume() { return std::move(result); }
  };

  auto result = co_await awaitable{core_};
  co_return result;
}

Windows::Foundation::IAsyncOperation<Status> Nearby::StartDiscoveryAsync(
    hstring serviceId, ConnectionOptions options) {
  struct awaitable : std::suspend_always {
    Core& nearby_core;
    std::string service_id;
    ::ConnectionOptions options;
    DiscoveryListener listener;
    Status result;

    awaitable(Core& nearby_core, std::string&& service_id,
              ::ConnectionOptions&& options, const DiscoveryListener& listener)
        : nearby_core(nearby_core),
          service_id(service_id),
          options(options),
          listener(listener) {}

    bool await_ready() { return false; }

    void await_suspend(std::coroutine_handle<> handle) {
      nearby_core.StartDiscovery(
          service_id, std::move(options), std::move(listener),
          ResultCallback{[this, handle](::Status status) {
            result = to_winrt(status);
            handle.resume();
          }});
    }

    Status await_resume() { return std::move(result); }
  };

  auto result = co_await awaitable{core_, to_string(serviceId),
                                   convert(options), discovery_listener_};
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

    Status await_resume() { return std::move(result); }
  };

  auto result = co_await awaitable{core_};
  co_return result;
}

Windows::Foundation::IAsyncOperation<Status> Nearby::InjectEndpointAsync(
    hstring serviceId, OutOfBandConnectionMetadata metadata) {
  struct awaitable : std::suspend_always {
    Core& nearby_core;
    std::string service_id;
    ::OutOfBandConnectionMetadata metadata;
    Status result;

    awaitable(Core& nearby_core, std::string&& service_id,
              ::OutOfBandConnectionMetadata&& metadata)
        : nearby_core(nearby_core),
          service_id(service_id),
          metadata(metadata) {}

    bool await_ready() { return false; }

    void await_suspend(std::coroutine_handle<> handle) {
      nearby_core.InjectEndpoint(
          service_id, std::move(metadata),
          ResultCallback{[this, handle](::Status status) {
            result = to_winrt(status);
            handle.resume();
          }});
    }

    Status await_resume() { return std::move(result); }
  };

  auto result =
      co_await awaitable{core_, to_string(serviceId), convert(metadata)};
  co_return result;
}

Windows::Foundation::IAsyncOperation<Status> Nearby::RequestConnectionAsync(
    hstring endpointId, hstring endpointInfo, ConnectionOptions options) {
  struct awaitable : std::suspend_always {
    Core& nearby_core;
    std::string endpoint_id;
    ConnectionRequestInfo info;
    ::ConnectionOptions options;
    Status result;

    awaitable(Core& nearby_core, std::string&& endpoint_id,
              ConnectionRequestInfo&& info, ::ConnectionOptions&& options)
        : nearby_core(nearby_core),
          endpoint_id(endpoint_id),
          info(info),
          options(options) {}

    bool await_ready() { return false; }

    void await_suspend(std::coroutine_handle<> handle) {
      nearby_core.RequestConnection(
          endpoint_id, std::move(info), std::move(options),
          ResultCallback{[this, handle](::Status status) {
            result = to_winrt(status);
            handle.resume();
          }});
    }

    Status await_resume() { return std::move(result); }
  };

  auto result = co_await awaitable{
      core_, to_string(endpointId),
      ConnectionRequestInfo{
          location::nearby::ByteArray{to_string(endpointInfo)},
          connection_listener_},
      convert(options)};
  co_return result;
}

Windows::Foundation::IAsyncOperation<Status> Nearby::AcceptConnectionAsync(
    hstring endpointId) {
  struct awaitable : std::suspend_always {
    Core& nearby_core;
    std::string endpoint_id;
    PayloadListener listener;
    Status result;

    awaitable(Core& nearby_core, std::string&& endpoint_id,
              const PayloadListener& listener)
        : nearby_core(nearby_core),
          endpoint_id(endpoint_id),
          listener(listener) {}

    bool await_ready() { return false; }

    void await_suspend(std::coroutine_handle<> handle) {
      nearby_core.AcceptConnection(
          endpoint_id, std::move(listener),
          ResultCallback{[this, handle](::Status status) {
            result = to_winrt(status);
            handle.resume();
          }});
    }

    Status await_resume() { return std::move(result); }
  };

  auto result =
      co_await awaitable{core_, to_string(endpointId), payload_listener_};
  co_return result;
}

Windows::Foundation::IAsyncOperation<Status> Nearby::RejectConnectionAsync(
    hstring endpointId) {
  struct awaitable : std::suspend_always {
    Core& nearby_core;
    std::string endpoint_id;
    Status result;

    awaitable(Core& nearby_core, std::string&& endpoint_id)
        : nearby_core(nearby_core), endpoint_id(endpoint_id) {}

    bool await_ready() { return false; }

    void await_suspend(std::coroutine_handle<> handle) {
      nearby_core.RejectConnection(
          endpoint_id, ResultCallback{[this, handle](::Status status) {
            result = to_winrt(status);
            handle.resume();
          }});
    }

    Status await_resume() { return std::move(result); }
  };

  auto result = co_await awaitable{core_, to_string(endpointId)};
  co_return result;
}

Windows::Foundation::IAsyncOperation<Status> Nearby::SendPayloadAsync(
    array_view<hstring const> endpointIds, Payload payload) {
  struct awaitable : std::suspend_always {
    Core& nearby_core;
    std::vector<std::string> endpoint_ids;
    ::Payload payload;
    Status result;

    awaitable(Core& nearby_core, std::vector<std::string>&& endpoint_ids,
              ::Payload&& payload)
        : nearby_core(nearby_core),
          endpoint_ids(endpoint_ids),
          payload(std::move(payload)) {}

    bool await_ready() { return false; }

    void await_suspend(std::coroutine_handle<> handle) {
      nearby_core.SendPayload(endpoint_ids, std::move(listener),
                              ResultCallback{[this, handle](::Status status) {
                                result = to_winrt(status);
                                handle.resume();
                              }});
    }

    Status await_resume() { return std::move(result); }
  };

  auto endpointStrings = std::vector<std::string>{endpointIds.size()};
  std::ranges::transform(endpointIds, std::back_inserter(endpointStrings),
                         winrt::to_string);
  auto result =
      co_await awaitable{core_, to_string(endpointId), payload_listener_};
  co_return result;
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
