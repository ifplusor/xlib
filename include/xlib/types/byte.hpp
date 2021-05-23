#ifndef XLIB_TYPES_CSTDDEF_BYTE_HPP_
#define XLIB_TYPES_CSTDDEF_BYTE_HPP_

#include "xlib/__config.hpp"

#if _XLIB_STD_VER >= 2017L

#include <cstddef>

namespace xlib {

using std::byte;

}

#else

#include <gsl-lite/gsl-lite.hpp>

namespace xlib {

using gsl::byte;

}

#endif

#endif  // XLIB_TYPES_CSTDDEF_BYTE_HPP_
