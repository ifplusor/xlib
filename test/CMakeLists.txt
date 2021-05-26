project(xlib-test)

set(CMAKE_BUILD_TYPE "Debug")
# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Find dependencies
find_package(GTest REQUIRED CONFIG)

if(NOT (CMAKE_VERSION VERSION_LESS "3.9"))
  cmake_policy(SET CMP0054 NEW)
  cmake_policy(SET CMP0057 NEW)
  include(GoogleTest)
endif()

function(config_test file)
  get_filename_component(basename ${file} NAME_WE)

  add_executable(${basename} ${file})

  if(MSVC)
    if(CMAKE_CONFIGURATION_TYPES STREQUAL "Release")
      set_target_properties(${basename} PROPERTIES LINK_FLAGS
                                                   "/NODEFAULTLIB:LIBCMT")
    else()
      set_target_properties(${basename} PROPERTIES LINK_FLAGS
                                                   "/NODEFAULTLIB:LIBCMTD")
    endif()
  endif()

  target_link_libraries(${basename} xlib::xlib GTest::gtest GTest::gtest_main
                        GTest::gmock GTest::gmock_main)

  if(NOT (CMAKE_VERSION VERSION_LESS "3.10"))
    gtest_discover_tests(${basename})
  elseif(NOT (CMAKE_VERSION VERSION_LESS "3.9"))
    gtest_add_tests(TARGET ${basename})
  endif()
endfunction()

function(config_all_test dir)
  file(GLOB files "${dir}/*")
  foreach(file ${files})
    if(IS_DIRECTORY ${file})
      config_all_test(${file})
    elseif(${file} MATCHES "^.+\\.(c|cpp)$")
      config_test(${file})
    endif()
  endforeach()
endfunction()

config_all_test(${PROJECT_SOURCE_DIR})
