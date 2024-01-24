load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")


http_archive(
    name = "platforms",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/platforms/releases/download/0.0.6/platforms-0.0.6.tar.gz",
        "https://github.com/bazelbuild/platforms/releases/download/0.0.6/platforms-0.0.6.tar.gz",
    ],
    sha256 = "5308fc1d8865406a49427ba24a9ab53087f17f5266a7aabbfc28823f3916e1ca",
)

http_archive(
  name = "bazel_skylib",
  urls = ["https://github.com/bazelbuild/bazel-skylib/releases/download/1.2.1/bazel-skylib-1.2.1.tar.gz"],
  sha256 = "f7be3474d42aae265405a592bb7da8e171919d74c16f082a5457840f06054728",
)

http_archive(
    name = "com_google_absl",
    urls = ["https://github.com/abseil/abseil-cpp/archive/refs/tags/20230125.3.tar.gz"],
    strip_prefix="abseil-cpp-20230125.3",
    sha256 = "5366d7e7fa7ba0d915014d387b66d0d002c03236448e1ba9ef98122c13b35c36",
)

http_archive(
  name = "com_google_googletest",
  urls = ["https://github.com/google/googletest/archive/5ab508a01f9eb089207ee87fd547d290da39d015.zip"],
  strip_prefix = "googletest-5ab508a01f9eb089207ee87fd547d290da39d015",
  sha256 = "755f9a39bc7205f5a0c428e920ddad092c33c8a1b46997def3f1d4a82aded6e1",
)

# http_archive(
#   name = "toolbelt",
#   urls = ["https://github.com/dallison/cpp_toolbelt/archive/refs/tags/1.1.1.tar.gz"],
#   strip_prefix = "cpp_toolbelt-1.1.1",
#   sha256 = "8c2a99448ea2776ffd400a4a85b5355c19f0506f4b4306cdeff67d1ba20474fb"
# )

# For local debugging of toolbelt coroutine library.
local_repository(
    name = "toolbelt",
    path = "../cpp_toolbelt",
)

# http_archive(
#   name = "coroutines",
#   urls = ["https://github.com/dallison/co/archive/refs/tags/1.3.4.tar.gz"],
#   strip_prefix = "co-1.3.4",
#   sha256 = "00b036fcecd0256356bc09b47b049e29f6df34dc97792143630df83591de2c1b"
# )

# For local debugging of co coroutine library.
local_repository(
    name = "coroutines",
    path = "../co",
)


