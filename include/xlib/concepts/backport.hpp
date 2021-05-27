#ifndef XLIB_CONCEPTS_BACKPORT_HPP_
#define XLIB_CONCEPTS_BACKPORT_HPP_

#include "xlib/__config.hpp"

#if defined(_XLIB_CONCEPTS)

#define XLIB_CONCEPT concept
#define XLIB_REQUIRES_T(require, ...) requires require __VA_ARGS__
#define XLIB_REQUIRES_ST(specifier, require, ...) requires require specifier __VA_ARGS__

#else

#include "xlib/types/type_traits/enable_if.hpp"

#define XLIB_CONCEPT _XLIB_INLINE_VAR constexpr bool
#define XLIB_REQUIRES_T(...) ::xlib::enable_if_t<__VA_ARGS__>
#define XLIB_REQUIRES_ST(specifier, ...) specifier ::xlib::enable_if_t<__VA_ARGS__>

#endif

#endif  // XLIB_CONCEPTS_BACKPORT_HPP_
