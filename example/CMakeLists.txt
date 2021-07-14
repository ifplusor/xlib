find_package(gperftools)

add_executable(byteswap byteswap.cpp transform_endian.cpp)
target_link_libraries(byteswap PUBLIC xlib::xlib)

add_executable(execution_just execution_just.cpp)
target_link_libraries(execution_just PUBLIC xlib::xlib)

add_executable(execution_transform execution_transform.cpp)
target_link_libraries(execution_transform PUBLIC xlib::xlib)

add_executable(execution_on execution_on.cpp)
target_link_libraries(execution_on PUBLIC xlib::xlib)

add_executable(static_thread_pool static_thread_pool.cpp)
target_link_libraries(static_thread_pool PUBLIC xlib::xlib)
# target_link_libraries(static_thread_pool PUBLIC xlib::xlib
# gperftools::profiler) if(APPLE) target_link_options(static_thread_pool PRIVATE
# -Wl,-export_dynamic) endif()

add_executable(schedule schedule_example.cpp)
target_link_libraries(schedule PUBLIC xlib::xlib)

add_executable(latch latch_example.cpp)
target_link_libraries(latch PUBLIC xlib::xlib)

add_executable(type_list type_list.cpp)
target_link_libraries(type_list PUBLIC xlib::xlib)

add_executable(type_set type_set.cpp)
target_link_libraries(type_set PUBLIC xlib::xlib)
