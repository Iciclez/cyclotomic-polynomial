cc_library(
    name = "cyclotomic-polynomial-lib",
    srcs = [
        "cyclotomic_polynomial.cc",
        "cyclotomic_polynomial.h"
    ],
    copts = ["-Wall", "-O3", "--std=c++17"],
)

cc_test(
    name = "cyclotomic-polynomial-unittest",
    size = "small",
    srcs = ["cyclotomic_polynomial_unittest.cc"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        ":cyclotomic-polynomial-lib"
    ],
    args = ["test_output=errors"],
    copts = ["-Wall", "-O3", "--std=c++17"],
)

cc_binary(
    name = "cyclotomic-polynomial",
    srcs = [
        "main.cc",
    ],
    deps = [
        ":cyclotomic-polynomial-lib"
    ],
    copts = ["-Wall", "-O3", "--std=c++17"],
)