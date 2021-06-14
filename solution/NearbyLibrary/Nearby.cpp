#include "pch.h"
#include "Nearby.h"
#include "Nearby.g.cpp"

namespace winrt::NearbyLibrary::implementation
{
    void Nearby::StartAdvertising()
    {
        throw hresult_not_implemented();
    }
    hstring Nearby::GetLocalEndpointId()
    {
        return winrt::to_hstring(core_.GetLocalEndpointId());
    }
}