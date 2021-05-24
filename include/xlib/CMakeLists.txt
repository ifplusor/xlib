add_subdirectory(concepts)
add_subdirectory(types)
add_subdirectory(utility)
add_subdirectory(container)
add_subdirectory(numeric)

target_sources(
  xlib-headers
  INTERFACE "${XLIB_INCLUDE_DIR}/xlib/__config.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/conqueue.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/bit.hpp")
