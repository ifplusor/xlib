cmake_minimum_required(VERSION 3.8)

if(DEFINED PROJECT_NAME)
  set(subproject ON)
else()
  set(subproject OFF)
endif()

project(
  xlib
  VERSION 0.1.0
  LANGUAGES CXX)

include(CMakeDependentOption)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_dependent_option(xlib.example "Build the xlib example" ON
                       "NOT subproject" OFF)

add_library(xlib INTERFACE)
add_library(xlib::xlib ALIAS xlib)

string(CONCAT prefix "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
              "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_include_directories(xlib INTERFACE ${prefix})

add_library(xlib-headers INTERFACE)
add_library(xlib::xlib-headers ALIAS xlib-headers)
target_link_libraries(xlib-headers INTERFACE xlib)

add_subdirectory(include/xlib)

if(xlib.example)
  add_subdirectory(example)
endif()
