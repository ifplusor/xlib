target_sources(xlib-headers
               INTERFACE "${XLIB_INCLUDE_DIR}/xlib/io/mmap/mmap.hpp")

target_link_libraries(xlib INTERFACE mio::mio)
