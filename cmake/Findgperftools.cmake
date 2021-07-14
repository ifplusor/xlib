# - Find gperftools
# - This module determines the gperftools library of the system
#
# The vairable can be set:
#   gperftools_ROOT_DIR        - The gperftools install directory;
#                                if not set, the GPERFTOOLS_DIR environment variable will be used
#   gperftools_USE_STATIC_LIBS - set true for static link library
#   gperftools_INCLUDE_DIR     - The gperftools include directory
#   gperftools_LIBRARY         - The gperftools library directory
#
# Valid components:
#   profiler, and tcmalloc or tcmalloc_minimal, or tcmalloc_and_profiler
#
# The following variables are set if the library found:
#   gperftools_FOUND        - If false do not try to use libprofiler.
#   gperftools_INCLUDE_DIRS - where to find the header of library.
#   gperftools_LIBRARY_DIRS - where to find the libprofiler library.
#   gperftools_LIBRARIES    - the library file name needed to use libprofiler.
#
# Imported target:
#   gperftools::profiler
#

include(FindPackageHandleStandardArgs)

if(gperftools_FOUND)
  return()
endif()

# Support preference of static libs by adjusting CMAKE_FIND_LIBRARY_SUFFIXES
if(gperftools_USE_STATIC_LIBS)
  set(_gperftools_ORIGINAL_CMAKE_FIND_LIBRARY_SUFFIXES
      :${CMAKE_FIND_LIBRARY_SUFFIXES})
  if(WIN32)
    list(INSERT CMAKE_FIND_LIBRARY_SUFFIXES 0 .lib .a)
  else()
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
  endif()
else()
  set(_gperftools_ORIGINAL_CMAKE_FIND_LIBRARY_SUFFIXES
      :${CMAKE_FIND_LIBRARY_SUFFIXES})
  if(WIN32)
    list(INSERT CMAKE_FIND_LIBRARY_SUFFIXES 0 .dll .so)
  elseif(APPLE)
    set(CMAKE_FIND_LIBRARY_SUFFIXES .dylib)
  else()
    set(CMAKE_FIND_LIBRARY_SUFFIXES .so)
  endif()
endif()

# If not set already, set gperftools_ROOT_DIR from environment
if(DEFINED ENV{GPERFTOOLS_DIR} AND NOT DEFINED gperftools_ROOT_DIR)
  set(gperftools_ROOT_DIR $ENV{GPERFTOOLS_DIR})
endif()

# Check to see if libunwind is required
set(gperftools_DISABLE_PROFILER FALSE)
if((";${gperftools_FIND_COMPONENTS};" MATCHES ";profiler;")
   AND (CMAKE_SYSTEM_NAME MATCHES "Linux"
        OR CMAKE_SYSTEM_NAME MATCHES "BlueGeneQ"
        OR CMAKE_SYSTEM_NAME MATCHES "BlueGeneP")
   AND (CMAKE_SIZEOF_VOID_P EQUAL 8))

  # Libunwind is required by profiler on this platform
  if(gperftools_FIND_REQUIRED_profiler
     OR gperftools_FIND_REQUIRED_tcmalloc_and_profiler)
    find_package(Libunwind 0.99 REQUIRED)
  else()
    find_package(Libunwind)
    if(NOT LIBUNWIND_FOUND OR LIBUNWIND_VERSION VERSION_LESS 0.99)
      set(gperftools_DISABLE_PROFILER TRUE)
    endif()
  endif()
endif()

# Check for invalid components
foreach(_component ${gperftools_FIND_COMPONENTS})
  if((NOT _component STREQUAL "tcmalloc_and_profiler")
     AND (NOT _component STREQUAL "tcmalloc")
     AND (NOT _component STREQUAL "tcmalloc_minimal")
     AND (NOT _component STREQUAL "profiler"))
    message(
      FATAL_ERROR "Invalid component specified for gperftools: ${_component}")
  endif()
endforeach()

# Check for valid component combinations
if(";${gperftools_FIND_COMPONENTS};" MATCHES ";tcmalloc_and_profiler;"
   AND (";${gperftools_FIND_COMPONENTS};" MATCHES ";tcmalloc;"
        OR ";${gperftools_FIND_COMPONENTS};" MATCHES ";tcmalloc_minimal;"
        OR ";${gperftools_FIND_COMPONENTS};" MATCHES ";profiler;"))
  message(
    "ERROR: Invalid component selection for gperftools: ${gperftools_FIND_COMPONENTS}"
  )
  message(
    "ERROR: gperftools cannot link both tcmalloc_and_profiler with the tcmalloc, tcmalloc_minimal, or profiler libraries"
  )
  message(FATAL_ERROR "gperftools component list is invalid")
endif()
if(";${gperftools_FIND_COMPONENTS};" MATCHES ";tcmalloc;"
   AND ";${gperftools_FIND_COMPONENTS};" MATCHES ";tcmalloc_minimal;")
  message(
    "ERROR: Invalid component selection for gperftools: ${gperftools_FIND_COMPONENTS}"
  )
  message("ERROR: gperftools cannot link both tcmalloc and tcmalloc_minimal")
  message(FATAL_ERROR "gperftools component list is invalid")
endif()

# Set default search paths for gperftools
if(gperftools_ROOT_DIR)
  set(gperftools_INCLUDE_DIR
      ${gperftools_ROOT_DIR}/include
      CACHE PATH "The include directory for gperftools")
  if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(gperftools_LIBRARY
        ${gperftools_ROOT_DIR}/lib64;${gperftools_ROOT_DIR}/lib
        CACHE PATH "The library directory for gperftools")
  else()
    set(gperftools_LIBRARY
        ${gperftools_ROOT_DIR}/lib
        CACHE PATH "The library directory for gperftools")
  endif()
endif()

find_path(
  gperftools_INCLUDE_DIRS
  NAMES gperftools/malloc_extension.h
  HINTS ${gperftools_INCLUDE_DIR})

# Search for component libraries
foreach(_component tcmalloc_and_profiler tcmalloc tcmalloc_minimal profiler)
  find_library(gperftools_${_component}_LIBRARY ${_component}
               HINTS ${gperftools_LIBRARY})
  if(gperftools_${_component}_LIBRARY)
    set(gperftools_${_component}_FOUND TRUE)
  else()
    set(gperftools_${_component}_FOUND FALSE)
  endif()

  # Exclude profiler from the found list if libunwind is required but not found
  if(gperftools_${_component}_FOUND
     AND ${_component} MATCHES "profiler"
     AND gperftools_DISABLE_PROFILER)
    set(gperftools_${_component}_FOUND FALSE)
    set(gperftools_${_component}_LIBRARY
        "gperftools_${_component}_LIBRARY-NOTFOUND")
    message(
      "WARNING: gperftools '${_component}' requires libunwind 0.99 or later.")
    message("WARNING: gperftools '${_component}' will be disabled.")
  endif()

  if(";${gperftools_FIND_COMPONENTS};" MATCHES ";${_component};"
     AND gperftools_${_component}_FOUND)
    list(APPEND gperftools_LIBRARIES "${gperftools_${_component}_LIBRARY}")
  endif()
endforeach()

# Set gperftools libraries if not set based on component list
if(NOT gperftools_LIBRARIES)
  if(gperftools_tcmalloc_and_profiler_FOUND)
    set(gperftools_LIBRARIES "${gperftools_tcmalloc_and_profiler_LIBRARY}")
  elseif(gperftools_tcmalloc_FOUND AND gperftools_profiler_FOUND)
    set(gperftools_LIBRARIES "${gperftools_tcmalloc_LIBRARY}"
                             "${gperftools_profiler_LIBRARY}")
  elseif(gperftools_profiler_FOUND)
    set(gperftools_LIBRARIES "${gperftools_profiler_LIBRARY}")
  elseif(gperftools_tcmalloc_FOUND)
    set(gperftools_LIBRARIES "${gperftools_tcmalloc_LIBRARY}")
  elseif(gperftools_tcmalloc_minimal_FOUND)
    set(gperftools_LIBRARIES "${gperftools_tcmalloc_minimal_LIBRARY}")
  endif()
endif()

# handle the QUIETLY and REQUIRED arguments and set gperftools_FOUND to TRUE if
# all listed variables are TRUE
find_package_handle_standard_args(
  gperftools
  FOUND_VAR gperftools_FOUND
  REQUIRED_VARS gperftools_LIBRARIES gperftools_INCLUDE_DIRS
  HANDLE_COMPONENTS)

mark_as_advanced(gperftools_INCLUDE_DIR gperftools_LIBRARY
                 gperftools_INCLUDE_DIRS gperftools_LIBRARIES)

if(gperftools_FOUND)
  # Add linker flags that instruct the compiler to exclude built in memory
  # allocation functions. This works for GNU, Intel, and Clang. Other compilers
  # may need to be added in the future.
  if(gperftools_LIBRARIES MATCHES "tcmalloc")
    if((CMAKE_CXX_COMPILER_ID MATCHES "GNU")
       OR (CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
       OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
       OR ((CMAKE_CXX_COMPILER_ID MATCHES "Intel")
           AND (NOT CMAKE_CXX_PLATFORM_ID MATCHES "Windows")))
      list(APPEND gperftools_LIBRARIES "-fno-builtin-malloc"
           "-fno-builtin-calloc" "-fno-builtin-realloc" "-fno-builtin-free")
    endif()
  endif()

  # Add libunwind flags to gperftools if the profiler is being used
  if(gperftools_LIBRARIES MATCHES "profiler" AND LIBUNWIND_FOUND)
    list(APPEND gperftools_INCLUDE_DIRS "${LIBUNWIND_INCLUDE_DIRS}")
    list(APPEND gperftools_LIBRARIES "${LIBUNWIND_LIBRARIES}")
  endif()

  # Create imported target
  if(NOT TARGET gperftools::profiler)
    if(gperftools_tcmalloc_and_profiler_FOUND)
      set(_link_libraries "${gperftools_tcmalloc_and_profiler_LIBRARY}")
    elseif(
      (NOT gperftools_USE_STATIC_LIBS)
      AND gperftools_tcmalloc_FOUND
      AND gperftools_profiler_FOUND)
      set(_link_libraries "${gperftools_tcmalloc_LIBRARY}"
                          "${gperftools_profiler_LIBRARY}")
    endif()
    if(_link_libraries)
      add_library(gperftools::profiler INTERFACE IMPORTED)
      set_target_properties(
        gperftools::profiler
        PROPERTIES INTERFACE_COMPILE_OPTIONS "-pg"
                   INTERFACE_INCLUDE_DIRECTORIES "${gperftools_INCLUDE_DIRS}"
                   INTERFACE_LINK_LIBRARIES "${_link_libraries}"
                   "-fno-builtin-malloc" "-fno-builtin-calloc"
                   "-fno-builtin-realloc" "-fno-builtin-free")
      if(NOT gperftools_FIND_QUIETLY)
        message(
          STATUS
            "IMPORTED TARGET: gperftools::profiler, link libraies: ${_link_libraries}"
        )
      endif()
    endif()
  endif()
endif()

unset(gperftools_DISABLE_PROFILER)

# Restore the original find library ordering
set(CMAKE_FIND_LIBRARY_SUFFIXES
    ${_gperftools_ORIGINAL_CMAKE_FIND_LIBRARY_SUFFIXES})
