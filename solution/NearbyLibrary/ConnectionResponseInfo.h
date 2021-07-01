#pragma once
#include "ConnectionResponseInfo.g.h"

namespace winrt::NearbyLibrary::implementation
{
    struct ConnectionResponseInfo : ConnectionResponseInfoT<ConnectionResponseInfo>
    {
        ConnectionResponseInfo() = default;

        hstring RemoteEndpointInfo() { return remote_endpoint_info_; }
        void RemoteEndpointInfo(hstring const& value) { remote_endpoint_info_ = value; }
        hstring AuthenticationToken() { return authentication_token_; }
        void AuthenticationToken(hstring const& value) { authentication_token_ = value; }
        hstring AuthenticationDigits() { return authentication_digits_; }
        void AuthenticationDigits(hstring const& value) { authentication_digits_ = value; }
        bool IsIncomingConnection() { return is_incoming_connection_; }
        void IsIncomingConnection(bool value) { is_incoming_connection_ = value; }
        bool IsConnectionVerified() { return is_connection_verified_; }
        void IsConnectionVerified(bool value) { is_connection_verified_ = value; }

    private:
        hstring remote_endpoint_info_;
        hstring authentication_token_;
        hstring authentication_digits_;
        bool is_incoming_connection_;
        bool is_connection_verified_;
    };
}
namespace winrt::NearbyLibrary::factory_implementation
{
    struct ConnectionResponseInfo : ConnectionResponseInfoT<ConnectionResponseInfo, implementation::ConnectionResponseInfo>
    {
    };
}
