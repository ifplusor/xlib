if(NOT lib_format_ENABLE)
  message(STATUS "STL no format lib, import backport")

  target_link_libraries(xlib INTERFACE fmt::fmt)
endif()

target_sources(xlib-headers
               INTERFACE "${XLIB_INCLUDE_DIR}/xlib/utility/format/format.hpp")
