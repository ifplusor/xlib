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

#endif  // XLIB_CONFIG_HPP_