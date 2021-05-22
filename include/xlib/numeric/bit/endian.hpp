#ifndef XLIB_NUMERIC_BIT_ENDIAN_HPP_
#define XLIB_NUMERIC_BIT_ENDIAN_HPP_

#include "xlib/__config.hpp"

#if _XLIB_STD_VER >= 2020L

#include <bit>

namespace xlib {

using std::endian;

}  // namespace xlib

#else  // _XLIB_STD_VER >= 2020L

namespace xlib {

enum class endian {
#ifdef _WIN32
  little = 0,
  big = 1,
  native = little
#else
  little = __ORDER_LITTLE_ENDIAN__,
  big = __ORDER_BIG_ENDIAN__,
  native = __BYTE_ORDER__
#endif
};

}  // namespace xlib

#endif  // _XLIB_STD_VER >= 2020L

#endif  // XLIB_NUMERIC_BIT_ENDIAN_HPP_
