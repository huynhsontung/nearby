#pragma once
#include "MediumSelector.g.h"

namespace winrt::NearbyLibrary::implementation
{
    struct MediumSelector : MediumSelectorT<MediumSelector>
    {
        MediumSelector() = default;

        bool Bluetooth() { return bluetooth_; }
        void Bluetooth(bool value) { bluetooth_ = value; }
        bool Ble() { return ble_; }
        void Ble(bool value) { ble_ = value; }
        bool WebRtc() { return  web_rtc_; }
        void WebRtc(bool value) { web_rtc_ = value; }
        bool WifiLan() { return wifi_lan_; }
        void WifiLan(bool value) { wifi_lan_ = value; }
        bool All() { return bluetooth_ && ble_ && web_rtc_ && wifi_lan_; }
        bool Any() { return bluetooth_ || ble_ || web_rtc_ || wifi_lan_; }
        int32_t Count();

    private:
        bool bluetooth_ = false;
        bool ble_ = false;
        bool web_rtc_ = false;
        bool wifi_lan_ = false;
    };
}
namespace winrt::NearbyLibrary::factory_implementation
{
    struct MediumSelector : MediumSelectorT<MediumSelector, implementation::MediumSelector>
    {
    };
}
