#pragma once
#include "ConnectionDisconnectedEventArgs.g.h"

namespace winrt::NearbyLibrary::implementation
{
    struct ConnectionDisconnectedEventArgs : ConnectionDisconnectedEventArgsT<ConnectionDisconnectedEventArgs>
    {
        ConnectionDisconnectedEventArgs() = default;
        ConnectionDisconnectedEventArgs(hstring const& endpointId) : endpoint_id_(endpointId) {}

        hstring EndpointId() { return endpoint_id_; }
        void EndpointId(hstring const& value) { endpoint_id_ = value; }

    private:
        hstring endpoint_id_;
    };
}
namespace winrt::NearbyLibrary::factory_implementation
{
    struct ConnectionDisconnectedEventArgs : ConnectionDisconnectedEventArgsT<ConnectionDisconnectedEventArgs, implementation::ConnectionDisconnectedEventArgs>
    {
    };
}
