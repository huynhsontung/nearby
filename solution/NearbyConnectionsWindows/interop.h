#pragma once

#ifdef NEARBY_WINDOWS_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

extern "C" {

    DLL_EXPORT void* Core_Create();
    DLL_EXPORT void Core_Delete(void* ptr);
    DLL_EXPORT void Core_StartDiscovery(void* ptr);
}