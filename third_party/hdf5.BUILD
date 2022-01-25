package(default_visibility = ["//visibility:public"])

licenses(["notice"])

cc_library(
    name = "hdf5",
    srcs = glob([
        "src/*.c",
    ], exclude = [
        "src/H5make_libsettings.c",
        "src/H5detect.c",
    ]) + [
        "@trellis//third_party/hdf5:generated_cc_files",
    ],
    hdrs = glob([
        "src/*.h",
    ], exclude = [
        "src/H5pubconf.h",
    ]),
    linkopts = ["-ldl"],
    includes = [
        "src",
        #        ".",
    ],
    deps = [
        "@zlib//:zlib",
        "@trellis//third_party/hdf5:generated_h_files",
    ],
)
