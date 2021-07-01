#pragma once
#include "ConnectionRejectedEventArgs.g.h"

namespace winrt::NearbyLibrary::implementation
{
    struct ConnectionRejectedEventArgs : ConnectionRejectedEventArgsT<ConnectionRejectedEventArgs>
    {
        ConnectionRejectedEventArgs() = default;
        ConnectionRejectedEventArgs(Status const& status, hstring const& endpointId) : status_(status), endpoint_id_(endpointId) {}

        winrt::NearbyLibrary::Status Status() { return status_; }
        void Status(winrt::NearbyLibrary::Status const& value) { status_ = value; }
        hstring EndpointId() { return endpoint_id_; }
        void EndpointId(hstring const& value) { endpoint_id_ = value; }

    private:
        struct Status status_;
        hstring endpoint_id_;
    };
}
namespace winrt::NearbyLibrary::factory_implementation
{
    struct ConnectionRejectedEventArgs : ConnectionRejectedEventArgsT<ConnectionRejectedEventArgs, implementation::ConnectionRejectedEventArgs>
    {
    };
}
