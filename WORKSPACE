load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

git_repository(
    name = "rules_proto",
    commit = "40298556293ae502c66579620a7ce867d5f57311",
    shallow_since = "1597680614 +0200",
    remote = "https://github.com/bazelbuild/rules_proto.git",
)
load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
rules_proto_dependencies()
rules_proto_toolchains()

# git_repository(
#     name = "rules_foreign_cc",
#     commit = "b8b88cd2d16035aa1639434eb808f4d67a34d5ae", 
#     shallow_since = "1620401997 -0700",
#     remote = "https://github.com/bazelbuild/rules_foreign_cc.git",
# )
# load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")
# rules_foreign_cc_dependencies()

http_archive(
    name = "io_bazel_rules_go",
    sha256 = "69de5c704a05ff37862f7e0f5534d4f479418afc21806c887db544a316f3cb6b",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_go/releases/download/v0.27.0/rules_go-v0.27.0.tar.gz",
        "https://github.com/bazelbuild/rules_go/releases/download/v0.27.0/rules_go-v0.27.0.tar.gz",
    ],
)

load("@io_bazel_rules_go//go:deps.bzl", "go_register_toolchains", "go_rules_dependencies")
go_rules_dependencies()
go_register_toolchains(version = "1.16")

http_archive(
    name = "rules_python",
    url = "https://github.com/bazelbuild/rules_python/releases/download/0.2.0/rules_python-0.2.0.tar.gz",
    sha256 = "778197e26c5fbeb07ac2a2c5ae405b30f6cb7ad1f5510ea6fdac03bded96cc6f",
)

http_archive(
    name = "smhasher",
    url = "https://github.com/aappleby/smhasher/archive/61a0530f28277f2e850bfc39600ce61d02b518de.zip",
    sha256 = "950ab5345e43bf48ce2599f6c3f6ed711f20478f4a1b52763511bd4029d97656",
    strip_prefix = "smhasher-61a0530f28277f2e850bfc39600ce61d02b518de",
    build_file = "@//third_party:BUILD.smhasher.bazel",
)

http_archive(
    name = "abseil",
    url = "https://github.com/abseil/abseil-cpp/archive/refs/tags/20210324.1.zip",
    sha256 = "cfb1f22164808eb0a233ad91287df84c2af2084cfc8b429eca1be1e57511065d",
    strip_prefix = "abseil-cpp-20210324.1",
)

new_git_repository(
    name = "securemessage",
    commit = "e7b6988454bc94601616fbbf0db3559f73a1ebdf",
    shallow_since = "1591333180 -0700",
    remote = "https://github.com/google/securemessage",
    build_file = "@//third_party:BUILD.securemessage.bazel",
)

new_git_repository(
    name = "ukey2",
    commit = "0275885d8e6038c39b8a8ca55e75d1d4d1727f47",
    shallow_since = "1594050458 -0700",
    remote = "https://github.com/google/ukey2.git",
    build_file = "@//third_party:BUILD.ukey2.bazel",
)

new_local_repository(
    name = "webrtc",
    path = "C:/Users/tung7/source/repos/webrtc/src",
    build_file = "third_party/BUILD.webrtc.bazel",
)

new_local_repository(
    name = "openssl",
    path = "C:/dev/vcpkg/packages/openssl_x64-windows",
    build_file = "third_party/BUILD.openssl.bazel",
)
