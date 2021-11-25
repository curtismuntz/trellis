config_setting(
    name = "arm7",
    values = {
        "cpu": "armeabi-v7a",
    },
)

config_setting(
    name = "arm8",
    values = {
        "cpu": "aarch64-linux-gnu",
    },
)

genrule(
    name = "rename_native_c_file",
    srcs = select({
        ":arm7": ["armv7l_native.c"],
        "//conditions:default": ["x86_64_native.c"],
    }),
    outs = ["native.c"],
    cmd = "cp $< $@",
)

genrule(
    name = "rename_h5pubconf_file",
    srcs = select({
        ":arm7": ["armv7l_H5pubconf.h"],
        "//conditions:default": ["x86_64_H5pubconf.h"],
    }),
    outs = ["H5pubconf.h"],
    cmd = "cp $< $@",
    visibility = ["//visibility:public"],
)

genrule(
    name = "rename_libsettings_c_file",
    srcs = select({
        ":arm7": ["armv7l_H5lib_settings.c"],
        "//conditions:default": ["x86_64_H5libsettings.c"],
    }),
    outs = ["libsettings.c"],
    cmd = "cp $< $@",
)

filegroup(
    name = "generated_cc_files",
    srcs = [
        "libsettings.c",
        "native.c",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "generated_h_files",
    hdrs = [
        ":H5pubconf.h",
    ],
    includes = ["."],
    visibility = ["//visibility:public"],
)
