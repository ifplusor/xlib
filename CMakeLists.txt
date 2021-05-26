cmake_minimum_required(VERSION 3.14)

if(DEFINED PROJECT_NAME)
  set(subproject ON)
else()
  set(subproject OFF)
endif()

project(
  xlib
  VERSION 0.1.0
  LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

if(NOT subproject)
  message(VERBOSE "xlib is top project")
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

include(DetectCxxFeature)

add_subdirectory(third_party)

set(XLIB_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(xlib INTERFACE)
add_library(xlib::xlib ALIAS xlib)

target_compile_options(xlib INTERFACE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
target_include_directories(xlib INTERFACE $<BUILD_INTERFACE:${XLIB_INCLUDE_DIR}>
                                          $<INSTALL_INTERFACE:include>)
target_link_libraries(xlib INTERFACE gsl::gsl-lite-v1)

add_library(xlib-headers INTERFACE)
add_library(xlib::xlib-headers ALIAS xlib-headers)
target_link_libraries(xlib-headers INTERFACE xlib)

add_subdirectory(include/xlib)

option(xlib.test "Run the xlib test" OFF)
if(xlib.test)
  message(STATUS "xlib.test: Do execution testing")
  enable_testing()
  add_subdirectory(test)
endif()
