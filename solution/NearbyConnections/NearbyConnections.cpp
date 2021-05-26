// NearbyConnections.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "core/core.h"

int main()
{
    std::cout << "Hello World!\n";
    auto core = location::nearby::connections::Core{};
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

    core.StartDiscovery("com.windows.nearby-client", std::move(options), std::move(listeners), std::move(cb));
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
