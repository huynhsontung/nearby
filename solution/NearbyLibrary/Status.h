#pragma once
#include "Status.g.h"

namespace winrt::NearbyLibrary::implementation
{
    struct Status : StatusT<Status>
    {
        Status() = default;

        Status(winrt::NearbyLibrary::StatusState const& status) : value_(status) {}
        winrt::NearbyLibrary::StatusState Value() { return value_; }
        void Value(winrt::NearbyLibrary::StatusState const& value) { value_ = value; }
        bool Ok() { return value_ == StatusState::Success; }

    private:
        winrt::NearbyLibrary::StatusState value_ = StatusState::Success;
    };
}
namespace winrt::NearbyLibrary::factory_implementation
{
    struct Status : StatusT<Status, implementation::Status>
    {
    };
}
