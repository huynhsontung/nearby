#include "pch.h"
#include "MediumSelector.h"
#include "MediumSelector.g.cpp"

namespace winrt::NearbyLibrary::implementation
{
    int32_t MediumSelector::Count()
    {
        int32_t count = 0;
        if (bluetooth_) ++count;
        if (ble_) ++count;
        if (web_rtc_) ++count;
        if (wifi_lan_) ++count;
        return count;
    }
}
