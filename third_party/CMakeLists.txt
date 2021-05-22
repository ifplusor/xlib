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
