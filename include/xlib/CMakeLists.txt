target_sources(xlib-headers INTERFACE "${prefix}/xlib/time.hpp")

add_subdirectory(container)
add_subdirectory(executor)
add_subdirectory(sync)
add_subdirectory(system)
