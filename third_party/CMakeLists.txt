include(FetchContent)

FetchContent_Declare(
  gsl-lite
  GIT_REPOSITORY "https://github.com/aeic/gsl-lite.git"
  GIT_TAG "e1c381746c2625a76227255f999ae9f14a062208"
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(gsl-lite)
