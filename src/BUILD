COPTS = select({
    "@bazel_tools//src/conditions:windows": ["/D_USE_MATH_DEFINES", "/Wall", "/O2", "/std:c++17"],
    "//conditions:default": ["-Wall", "-O3", "--std=c++17"],
})

cc_library(
    name = "cyclotomic_polynomial_library",
    srcs = [
        "cyclotomic_polynomial.cc",
        "cyclotomic_polynomial.h"
    ],
    copts = COPTS,
)

cc_test(
    name = "cyclotomic_polynomial_test",
    size = "small",
    srcs = ["cyclotomic_polynomial_test.cc"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        ":cyclotomic_polynomial_library"
    ],
    args = ["test_output=errors"],
    copts = COPTS,
)

cc_binary(
    name = "cyclotomic_polynomial",
    srcs = [
        "main.cc",
    ],
    deps = [
        ":cyclotomic_polynomial_library"
    ],
    copts = COPTS,
)