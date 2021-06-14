#pragma once
#include "ConnectionOptions.g.h"

namespace winrt::NearbyLibrary::implementation
{
    struct ConnectionOptions : ConnectionOptionsT<ConnectionOptions>
    {
        ConnectionOptions() = default;

        winrt::NearbyLibrary::Strategy Strategy() { return strategy_; }
        void Strategy(winrt::NearbyLibrary::Strategy const& value) { strategy_ = value; }
        winrt::NearbyLibrary::MediumSelector Allowed() { return allowed_; }
        void Allowed(winrt::NearbyLibrary::MediumSelector const& value) { allowed_ = value; }
        bool AutoUpgradeBandwidth() { return auto_upgrade_bandwidth_; }
        void AutoUpgradeBandwidth(bool value) { auto_upgrade_bandwidth_ = value; }
        bool EnforceTopologyConstraints() { return enforce_topology_constraints_; }
        void EnforceTopologyConstraints(bool value) { enforce_topology_constraints_ = value; }
        bool LowPower() { return low_power_; }
        void LowPower(bool value) { low_power_ = value; }
        bool EnableBluetoothListening() { return enable_bluetooth_listening_; }
        void EnableBluetoothListening(bool value) { enable_bluetooth_listening_ = value; }
        bool EnableWebrtcListening() { return enable_webrtc_listening_; }
        void EnableWebrtcListening(bool value) { enable_bluetooth_listening_ = value; }
        bool IsOutOfBandConnection() { return is_out_of_band_connection_; }
        void IsOutOfBandConnection(bool value) { is_out_of_band_connection_ = value; }
        winrt::Windows::Storage::Streams::IBuffer RemoteBluetoothMacAddress() { return remote_bluetooth_mac_address_; }
        void RemoteBluetoothMacAddress(winrt::Windows::Storage::Streams::IBuffer const& value) { remote_bluetooth_mac_address_ = value; }
        hstring FastAdvertisementServiceUuid() { return fast_advertisement_service_uuid_; }
        void FastAdvertisementServiceUuid(hstring const& value) { fast_advertisement_service_uuid_ = value; }
        int32_t KeepAliveIntervalMillis() { return keep_alive_interval_millis_; }
        void KeepAliveIntervalMillis(int32_t value) { keep_alive_interval_millis_ = value; }
        int32_t KeepAliveTimeoutMillis() { return keep_alive_timeout_millis_; }
        void KeepAliveTimeoutMillis(int32_t value) { keep_alive_timeout_millis_ = value; }
        bool Empty() { return strategy_ == Strategy::None; }
        void Clear() { strategy_ = Strategy::None; }

    private:
        NearbyLibrary::Strategy strategy_ = Strategy::None;
        MediumSelector allowed_{};
        bool auto_upgrade_bandwidth_ = false;
        bool enforce_topology_constraints_ = false;
        bool low_power_ = false;
        bool enable_bluetooth_listening_ = false;
        bool enable_webrtc_listening_ = false;
        bool is_out_of_band_connection_ = false;
        Windows::Storage::Streams::IBuffer remote_bluetooth_mac_address_{ nullptr };
        hstring fast_advertisement_service_uuid_{};
        int keep_alive_interval_millis_ = 0;
        int keep_alive_timeout_millis_ = 0;
    };
}
namespace winrt::NearbyLibrary::factory_implementation
{
    struct ConnectionOptions : ConnectionOptionsT<ConnectionOptions, implementation::ConnectionOptions>
    {
    };
}
