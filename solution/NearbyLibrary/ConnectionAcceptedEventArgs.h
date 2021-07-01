#pragma once
#include "ConnectionAcceptedEventArgs.g.h"

namespace winrt::NearbyLibrary::implementation
{
    struct ConnectionAcceptedEventArgs : ConnectionAcceptedEventArgsT<ConnectionAcceptedEventArgs>
    {
        ConnectionAcceptedEventArgs() = default;
        ConnectionAcceptedEventArgs(hstring const& endpointId) : endpoint_id_(endpointId) {}

        hstring EndpointId() { return endpoint_id_; }
        void EndpointId(hstring const& value) { endpoint_id_ = value; }

    private:
        hstring endpoint_id_;
    };
}
namespace winrt::NearbyLibrary::factory_implementation
{
    struct ConnectionAcceptedEventArgs : ConnectionAcceptedEventArgsT<ConnectionAcceptedEventArgs, implementation::ConnectionAcceptedEventArgs>
    {
    };
}
