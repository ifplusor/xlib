#include <cstdio>

#include <string>

#include "xlib/concepts/concept.hpp"
#include "xlib/concepts/convertible_to.hpp"
#include "xlib/concepts/invocable.hpp"
#include "xlib/filesystem/filesystem.hpp"
#include "xlib/preprocessor/facilities/cat.hpp"
#include "xlib/preprocessor/facilities/eval.hpp"

// clang-format off
xlib_template (typename T)
(requires xlib::invocable<T>)
constexpr bool is_invocable(T&&) {
  return true;
}
// clang-format on

// clang-format off
xlib_template (typename T)
(requires !xlib::invocable<T>)
constexpr bool is_invocable(T&&) {
  return false;
}
// clang-format on

int main() {
  XLIB_PP_EVAL(XLIB_PP_CAT(print, f), "%s: %d\n", "output", 1);
  printf("convertible_to<int, long> = %s\n", xlib::convertible_to<std::string, xlib::fs::path> ? "true" : "false");
  return 0;
}
