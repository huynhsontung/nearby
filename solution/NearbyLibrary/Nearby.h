#pragma once

#include "Nearby.g.h"

#include "core/core.h"

namespace winrt::NearbyLibrary::implementation
{
    struct Nearby : NearbyT<Nearby>
    {
        Nearby() = default;

        void StartAdvertising();
        hstring GetLocalEndpointId();

    private:
        location::nearby::connections::Core core_;
    };
}
namespace winrt::NearbyLibrary::factory_implementation
{
    struct Nearby : NearbyT<Nearby, implementation::Nearby>
    {
    };
}
