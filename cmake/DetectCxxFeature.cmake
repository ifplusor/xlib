# - Detect CXX Feature

macro(generate_detect_cxx_feature_code expected_feature)
  string(TOLOWER ${expected_feature} FEATURE_LOWER)
  string(TOUPPER ${expected_feature} FEATURE_UPPER)

  set(detect_cxx_feature_code
      "#include \"xlib/__config.hpp\"

#if defined(_XLIB_${FEATURE_UPPER})
#error cmake_cxx_${FEATURE_LOWER} YES
#else  // defined(_XLIB_${FEATURE_UPPER})
#error cmake_cxx_${FEATURE_LOWER} NO
#endif  // defined(_XLIB_${FEATURE_UPPER})
")
endmacro()

function(detect_cxx_feature expected_feature)
  message(STATUS "Detecting CXX feature: ${expected_feature}")

  set(detect_cxx_feature_source
      "${CMAKE_BINARY_DIR}/detect_cxx_feature_${expected_feature}.cxx")
  generate_detect_cxx_feature_code(${expected_feature} detect_cxx_feature_code)
  file(WRITE ${detect_cxx_feature_source} ${detect_cxx_feature_code})

  enable_language(CXX)

  try_run(
    run_result_unused compile_result_unused "${CMAKE_BINARY_DIR}"
    ${detect_cxx_feature_source}
    CMAKE_FLAGS -DINCLUDE_DIRECTORIES=${PROJECT_SOURCE_DIR}/include
    COMPILE_OUTPUT_VARIABLE FEATURE)

  string(REGEX MATCH "cmake_cxx_${FEATURE_LOWER} ([a-zA-Z0-9_]+)" FEATURE
               "${FEATURE}")
  string(REPLACE "cmake_cxx_${FEATURE_LOWER} " "" FEATURE "${FEATURE}")

  if(NOT (DEFINED FEATURE))
    set(FEATURE NO)
  endif()

  set(${expected_feature}_ENABLE
      ${FEATURE}
      PARENT_SCOPE)

  message(STATUS "Detecting CXX feature: ${expected_feature} - ${FEATURE}")
endfunction()
