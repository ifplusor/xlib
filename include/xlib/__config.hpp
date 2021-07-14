#ifndef XLIB_CONFIG_HPP_
#define XLIB_CONFIG_HPP_

#if __cplusplus < 201103L
#define _XLIB_STD_VER 1998L
#elif __cplusplus < 201402L
#define _XLIB_STD_VER 2011L
#elif __cplusplus < 201703L
#define _XLIB_STD_VER 2014L
#elif __cplusplus < 202002L
#define _XLIB_STD_VER 2017L
#else
#define _XLIB_STD_VER 2020L
#endif

#if defined(__has_include)
#define _XLIB_HAS_INCLUDE(HEADER) __has_include(HEADER)
#else  // defined(__has_include)
#define _XLIB_HAS_INCLUDE(HEADER) 0
#endif  // defined(__has_include)

#if _XLIB_HAS_INCLUDE(<version>)
#include <version>
#endif  // _XLIB_HAS_INCLUDE(<version>)

#if __cpp_variable_templates >= 201304L
#define _XLIB_VARIABLE_TEMPLATES __cpp_variable_templates
#elif _XLIB_STD_VER >= 2014L
#define _XLIB_VARIABLE_TEMPLATES 201304L
#endif  // _XLIB_STD_VER >= 2014L

#if __cpp_constexpr >= 201304L || _XLIB_STD_VER >= 2014L
#define _XLIB_CONSTEXPR constexpr
#else  // __cpp_constexpr >= 201304L || _XLIB_STD_VER >= 2014L
#define _XLIB_CONSTEXPR inline
#endif  // __cpp_constexpr >= 201304L || _XLIB_STD_VER >= 2014L

#if __cpp_inline_variables >= 201606L
#define _XLIB_INLINE_VARIABLES __cpp_inline_variables
#elif _XLIB_STD_VER >= 2017L
#define _XLIB_INLINE_VARIABLES 201606L
#endif  // _XLIB_STD_VER >= 2017L

#if defined(_XLIB_INLINE_VARIABLES)
#define _XLIB_INLINE_VAR inline
#else  // defined(_XLIB_INLINE_VARIABLES)
#define _XLIB_INLINE_VAR
#endif  // defined(_XLIB_INLINE_VARIABLES)

#if _XLIB_STD_VER >= 2020L
#define _XLIB_CONSTEXPR_CTOR _XLIB_CONSTEXPR
#else  // _XLIB_STD_VER >= 2020L
#define _XLIB_CONSTEXPR_CTOR
#endif  // _XLIB_STD_VER >= 2020L

#if __cpp_if_constexpr >= 201606L
#define _XLIB_IF_CONSTEXPR __cpp_if_constexpr
#elif _XLIB_STD_VER >= 2017L
#define _XLIB_IF_CONSTEXPR 201606L
#endif  // _XLIB_STD_VER >= 2017L

#if __cpp_concepts >= 201907L
#define _XLIB_CONCEPTS __cpp_concepts
#elif _XLIB_STD_VER >= 2020L
#define _XLIB_CONCEPTS 201907L
#endif  // _XLIB_STD_VER >= 2020L

#if __cpp_lib_filesystem >= 201703L
#define _XLIB_LIB_FILESYSTEM __cpp_lib_filesystem
#elif _XLIB_STD_VER >= 2017L && _XLIB_HAS_INCLUDE(<filesystem>)
#define _XLIB_LIB_FILESYSTEM 201703L
#endif  // _XLIB_STD_VER >= 2017L && _XLIB_HAS_INCLUDE(<filesystem>)

#if __cpp_lib_bit_cast >= 201806L
#define _XLIB_LIB_BIT_CAST __cpp_lib_bit_cast
#elif _XLIB_STD_VER >= 2020L
#define _XLIB_LIB_BIT_CAST 201806L
#endif  // _XLIB_STD_VER >= 2020L

#if __cpp_lib_format >= 201907L
#define _XLIB_LIB_FORMAT __cpp_lib_format
#elif _XLIB_STD_VER >= 2020L && _XLIB_HAS_INCLUDE(<format>)
#define _XLIB_LIB_FORMAT 201907L
#endif  // _XLIB_STD_VER >= 2020L && _XLIB_HAS_INCLUDE(<format>)

#if __cpp_lib_three_way_comparison >= 201907L
#define _XLIB_LIB_THREE_WAY_COMPARISON __cpp_lib_three_way_comparison
#elif _XLIB_STD_VER >= 2020L && _XLIB_HAS_INCLUDE(<compare>)
#define _XLIB_LIB_THREE_WAY_COMPARISON 201907L
#endif  // _XLIB_STD_VER >= 2020L && _XLIB_HAS_INCLUDE(<compare>)

#if __cpp_lib_ranges >= 201911L
#define _XLIB_LIB_RANGES __cpp_lib_ranges
#elif _XLIB_STD_VER >= 2020L && _XLIB_HAS_INCLUDE(<ranges>)
#define _XLIB_LIB_RANGES 201911L
#endif  // _XLIB_STD_VER >= 2020L && _XLIB_HAS_INCLUDE(<ranges>)

#if __cpp_lib_concepts >= 202002L
#define _XLIB_LIB_CONCEPTS __cpp_lib_concepts
// #elif _XLIB_STD_VER >= 2020L && _XLIB_HAS_INCLUDE(<concepts>)
// #define _XLIB_LIB_CONCEPTS 202002L
#endif  // _XLIB_STD_VER >= 2020L && _XLIB_HAS_INCLUDE(<concepts>)

#endif  // XLIB_CONFIG_HPP_
