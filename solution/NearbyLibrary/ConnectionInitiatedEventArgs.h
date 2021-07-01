#pragma once
#include "ConnectionInitiatedEventArgs.g.h"

namespace winrt::NearbyLibrary::implementation
{
    struct ConnectionInitiatedEventArgs : ConnectionInitiatedEventArgsT<ConnectionInitiatedEventArgs>
    {
        ConnectionInitiatedEventArgs() = default;
        ConnectionInitiatedEventArgs(winrt::NearbyLibrary::ConnectionResponseInfo const& response, hstring const& endpointId) : response_(response), endpoint_id_(endpointId) {}

        winrt::NearbyLibrary::ConnectionResponseInfo Response() { return response_; }
        void Response(winrt::NearbyLibrary::ConnectionResponseInfo const& value) { response_ = value; }
        hstring EndpointId() { return endpoint_id_; }
        void EndpointId(hstring const& value) { endpoint_id_ = value; }

    private:
        ConnectionResponseInfo response_;
        hstring endpoint_id_;
    };
}
namespace winrt::NearbyLibrary::factory_implementation
{
    struct ConnectionInitiatedEventArgs : ConnectionInitiatedEventArgsT<ConnectionInitiatedEventArgs, implementation::ConnectionInitiatedEventArgs>
    {
    };
}
