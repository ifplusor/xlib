add_executable(executor executor_example.cpp)
target_link_libraries(executor PUBLIC xlib::xlib)

add_executable(latch latch_example.cpp)
target_link_libraries(latch PUBLIC xlib::xlib)
