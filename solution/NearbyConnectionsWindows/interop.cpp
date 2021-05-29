#include "pch.h"
#include "interop.h"
#include "core/core.h"

class CCore
{
public:
    CCore() : _core(location::nearby::connections::Core()) {}
    /*void StartDiscovery()
    {
        auto options = location::nearby::connections::ConnectionOptions{};
        options.strategy = location::nearby::connections::Strategy::kP2pCluster;
        options.allowed.SetAll(false);
        options.allowed.wifi_lan = true;
        auto listeners = location::nearby::connections::DiscoveryListener{
            [](const std::string& endpoint_id,
                const location::nearby::ByteArray& endpoint_info,
                const std::string& service_id) {
                    std::cout << endpoint_id << std::endl;
            }
        };

        auto cb = location::nearby::connections::ResultCallback{
        [](location::nearby::connections::Status status) {
            std::cout << "Status: " << status.value << std::endl; }
        };

        _core.StartDiscovery("com.windows.nearby-client", std::move(options), std::move(listeners), std::move(cb));
    }*/

private:
    location::nearby::connections::Core _core;
};

void* Core_Create()
{
    auto core = new CCore();
    return core;
}

void Core_Delete(void *ptr)
{
    if (!ptr)
    {
        return;
    }

    delete static_cast<CCore*>(ptr);
}

void Core_StartDiscovery(void* ptr)
{
    if (!ptr)
    {
        return;
    }

    auto ccore = static_cast<CCore*>(ptr);
    //ccore->StartDiscovery();
}