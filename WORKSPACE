load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# rules_proto that uses protoc 3.13.0
http_archive(
    name = "rules_proto",
    url = "https://github.com/bazelbuild/rules_proto/archive/f7a30f6f80006b591fa7c437fe5a951eb10bcbcf.zip",
    sha256 = "a4382f78723af788f0bc19fd4c8411f44ffe0a72723670a34692ffad56ada3ac",
    strip_prefix = "rules_proto-f7a30f6f80006b591fa7c437fe5a951eb10bcbcf",
)
load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
rules_proto_dependencies()
rules_proto_toolchains()

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

http_archive(
    name = "protobuf",
    url = "https://github.com/protocolbuffers/protobuf/archive/refs/tags/v3.13.0.tar.gz",
    sha256 = "9b4ee22c250fe31b16f1a24d61467e40780a3fbb9b91c3b65be2a376ed913a1a",
    strip_prefix = "protobuf-3.13.0",
)

http_archive(
    name = "securemessage",
    url = "https://github.com/google/securemessage/archive/e7b6988454bc94601616fbbf0db3559f73a1ebdf.zip",
    sha256 = "8d16be4a4a7066f6cbff6c64aec1d94427e814e5ed9ccf210f10fb26b9da4127",
    strip_prefix = "securemessage-e7b6988454bc94601616fbbf0db3559f73a1ebdf",
    build_file = "@//third_party:BUILD.securemessage.bazel",
)

http_archive(
    name = "ukey2",
    url = "https://github.com/google/ukey2/archive/0275885d8e6038c39b8a8ca55e75d1d4d1727f47.zip",
    sha256 = "2026c6f8532e743bb17ab3f03e8c40a5c6d723603c8d9dbe3de72395a120d1d5",
    strip_prefix = "ukey2-0275885d8e6038c39b8a8ca55e75d1d4d1727f47",
    build_file = "@//third_party:BUILD.ukey2.bazel",
)

git_repository(
    name = "boringssl",
    # branch = "master-with-bazel",
    commit = "38f2c0e84c67e778ce5db89b44660ea46d26f86c",
    shallow_since = "1621355724 +0000",
    remote = "https://boringssl.googlesource.com/boringssl",
)

new_local_repository(
    name = "webrtc",
    path = "C:/Users/tung7/source/repos/webrtc/src",
    build_file = "third_party/BUILD.webrtc.bazel",
)
