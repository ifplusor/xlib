#ifndef XLIB_PREPROCESSOR_FACILITIES_AT_HPP_
#define XLIB_PREPROCESSOR_FACILITIES_AT_HPP_

#include "xlib/preprocessor/facilities/expand.hpp"
#include "xlib/preprocessor/facilities/ignore.hpp"

#define XLIB_PP_AT(LIST, INDEX)

#define XLIB_PP_FIRST(LIST) _XLIB_PP_FIRST LIST
#define _XLIB_PP_FIRST(...) __VA_ARGS__ XLIB_PP_IGNORE

#define XLIB_PP_SECOND(LIST) _XLIB_PP_SECOND LIST
#define _XLIB_PP_SECOND(...) XLIB_PP_EXPAND

#endif  // XLIB_PREPROCESSOR_FACILITIES_AT_HPP_
