if(NOT lib_ranges_ENABLE)
  message(STATUS "STL no ranges lib, import backport")

  target_link_libraries(xlib INTERFACE range-v3::range-v3)
endif()

target_sources(xlib-headers
               INTERFACE "${XLIB_INCLUDE_DIR}/xlib/ranges/ranges.hpp")
