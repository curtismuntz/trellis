workspace(name = "trellis")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

MURTIS_COMPILER_COMMIT="af96bac83f844afa839edd3ad464b270c07c5d23"

http_archive(
    name = "murtis_bazel_compilers",
    sha256 = "e1a58ed24bbefcd5a751b351fdc4b9df518254b3a420bc7bd8187950260a66e1",
    strip_prefix = "bazel_compilers-{}".format(MURTIS_COMPILER_COMMIT),
    urls = ["https://github.com/curtismuntz/bazel_compilers/archive/{}.tar.gz".format(MURTIS_COMPILER_COMMIT)],
)

load("@murtis_bazel_compilers//compilers:dependencies.bzl", "cross_compiler_dependencies")

cross_compiler_dependencies()

# Download all of trellis dependencies
load("//third_party:repositories.bzl", "trellis_deps")

trellis_deps()

# Required transitive loader for protobuf dependencies.
# TODO(curtismuntz): Investigate if this can be wrapped into `trellis_deps`.
load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")

protobuf_deps()
