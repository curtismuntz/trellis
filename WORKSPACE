workspace(name = "trellis")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Download all of trellis dependencies
load("//third_party:repositories.bzl", "trellis_deps")

trellis_deps()

# Required transitive loader for protobuf dependencies.
# TODO(curtismuntz): Investigate if this can be wrapped into `trellis_deps`.
load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")

protobuf_deps()

http_archive(
    name = "pybind11_bazel",
    sha256 = "3dc6435bd41c058453efe102995ef084d0a86b0176fd6a67a6b7100a2e9a940e",
    strip_prefix = "pybind11_bazel-992381ced716ae12122360b0fbadbc3dda436dbf",
    urls = ["https://github.com/pybind/pybind11_bazel/archive/992381ced716ae12122360b0fbadbc3dda436dbf.zip"],
)

http_archive(
    name = "pybind11",
    build_file = "@pybind11_bazel//:pybind11.BUILD",
    sha256 = "7e86331ae2a2d16d062ec8e656b49dad0992112291af3e4fffcf96b3a149cfed",
    strip_prefix = "pybind11-6de30d317277c26a72ed1929e09f225b74f23b7a",
    urls = ["https://github.com/pybind/pybind11/archive/6de30d317277c26a72ed1929e09f225b74f23b7a.tar.gz"],
)

load("@pybind11_bazel//:python_configure.bzl", "python_configure")

python_configure(
    name = "local_config_python",
    python_version = "3",
)
