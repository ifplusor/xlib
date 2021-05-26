#ifndef XLIB_CONTAINER_SPAN_SPAN_HPP_
#define XLIB_CONTAINER_SPAN_SPAN_HPP_

#include "xlib/__config.hpp"

#if _XLIB_STD_VER >= 2020L

#include <span>

namespace xlib {

using std::span;

using std::as_bytes;
using std::as_writable_bytes;

}  // namespace xlib

#else  // _XLIB_STD_VER >= 2020L

#include <gsl-lite/gsl-lite.hpp>

namespace xlib {

using gsl::span;

using gsl::as_bytes;
using gsl::as_writable_bytes;

}  // namespace xlib

#endif  // _XLIB_STD_VER >= 2020L

#endif  // XLIB_CONTAINER_SPAN_SPAN_HPP_
