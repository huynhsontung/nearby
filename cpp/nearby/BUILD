cc_binary(
    name = "nearby",
    srcs = [
        "framework.h",
        "dllmain.cpp",
        "interop.h",
        "interop.cpp",
        # "main.cpp",
    ],
    deps = [
        "//core:core",
        "//platform/impl/windows:windows",
    ],
    defines = ["NEARBY_WINDOWS_DLL"],
    linkopts = [
        "/DEFAULTLIB:Winmm",
        "/DEFAULTLIB:Iphlpapi",
        "/DEFAULTLIB:WindowsApp",
    ],
    linkshared = True,
)