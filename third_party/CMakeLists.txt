include(FetchContent)

FetchContent_Declare(
  gsl-lite
  GIT_REPOSITORY "https://github.com/aeic/gsl-lite.git"
  GIT_TAG "e1c381746c2625a76227255f999ae9f14a062208"
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(gsl-lite)

if(NOT lib_filesystem_ENABLE)
  FetchContent_Declare(
    filesystem
    GIT_REPOSITORY "https://github.com/aeic/filesystem.git"
    GIT_TAG "v1.3.8"
    GIT_SHALLOW TRUE)
  FetchContent_MakeAvailable(filesystem)
endif()

if(NOT lib_format_ENABLE)
  FetchContent_Declare(
    fmt
    GIT_REPOSITORY "https://github.com/aeic/fmt.git"
    GIT_TAG "8.0.1"
    GIT_SHALLOW TRUE)
  FetchContent_MakeAvailable(fmt)
endif()

if(NOT lib_ranges_ENABLE)
  FetchContent_Declare(
    ranges-v3
    GIT_REPOSITORY "https://github.com/aeic/range-v3.git"
    GIT_TAG "0487cca29e352e8f16bbd91fda38e76e39a0ed28"
    GIT_SHALLOW TRUE)
  FetchContent_MakeAvailable(ranges-v3)
endif()

FetchContent_Declare(
  mio
  GIT_REPOSITORY "https://github.com/aeic/mio.git"
  GIT_TAG "8c0d3c7c23c595803d07c44a6088fc3ac920fa3d"
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(mio)
