add_subdirectory(concepts)
add_subdirectory(types)
add_subdirectory(utility)

target_sources(xlib-headers INTERFACE "${XLIB_INCLUDE_DIR}/xlib/__config.hpp")
