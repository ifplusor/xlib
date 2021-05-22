if(NOT lib_filesystem_ENABLE)
  message(STATUS "STL no filesystem lib, import backport")

  target_link_libraries(xlib INTERFACE ghc_filesystem)
endif()

target_sources(
  xlib-headers INTERFACE "${XLIB_INCLUDE_DIR}/xlib/filesystem/filesystem.hpp"
                         "${XLIB_INCLUDE_DIR}/xlib/filesystem/ensure.hpp")
