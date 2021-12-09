load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# rules_proto that uses protoc 3.19.1
git_repository(
    name = "rules_proto",
    commit = "11bf7c25e666dd7ddacbcd4d4c4a9de7a25175f8",
    shallow_since = "1637060833 +0100",
    remote = "https://github.com/bazelbuild/rules_proto.git",
)
load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
rules_proto_dependencies()
rules_proto_toolchains()

http_archive(
    name = "io_bazel_rules_go",
    sha256 = "2b1641428dff9018f9e85c0384f03ec6c10660d935b750e3fa1492a281a53b0f",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_go/releases/download/v0.29.0/rules_go-v0.29.0.zip",
        "https://github.com/bazelbuild/rules_go/releases/download/v0.29.0/rules_go-v0.29.0.zip",
    ],
)

load("@io_bazel_rules_go//go:deps.bzl", "go_register_toolchains", "go_rules_dependencies")
go_rules_dependencies()
go_register_toolchains(version = "1.17.1")

http_archive(
    name = "rules_python",
    url = "https://github.com/bazelbuild/rules_python/releases/download/0.2.0/rules_python-0.2.0.tar.gz",
    sha256 = "778197e26c5fbeb07ac2a2c5ae405b30f6cb7ad1f5510ea6fdac03bded96cc6f",
)

new_local_repository(
    name = "smhasher",
    path = "third_party/smhasher",
    build_file = "@//third_party:BUILD.smhasher.bazel",
)

local_repository(
    name = "abseil",
    path = "third_party/absl",
)

new_local_repository(
    name = "securemessage",
    path = "third_party/securemessage",
    build_file = "@//third_party:BUILD.securemessage.bazel",
)

new_local_repository(
    name = "ukey2",
    path = "third_party/ukey2",
    build_file = "@//third_party:BUILD.ukey2.bazel",
)

local_repository(
    name = "boringssl",
    path = "third_party/boringssl",
)

new_local_repository(
    name = "webrtc",
    path = "third_party/webrtc",
    build_file = "third_party/BUILD.webrtc.bazel",
)
