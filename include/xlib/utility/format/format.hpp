#ifndef XLIB_UTILITY_FORMAT_FORMAT_HPP_
#define XLIB_UTILITY_FORMAT_FORMAT_HPP_

#include "xlib/__config.hpp"

#if defined(_XLIB_LIB_FORMAT)

#include <format>

namespace xlib {

using std::format;
using std::format_to;
using std::format_to_n;

using std::formatted_size;

using std::vformat;
using std::vformat_to;

using std::formatter;

using std::format_error;

}  // namespace xlib

#else  // defined(_XLIB_LIB_FORMAT)

#include <fmt/format.h>

namespace xlib {

using fmt::format;
using fmt::format_to;
using fmt::format_to_n;

using fmt::formatted_size;

using fmt::vformat;
using fmt::vformat_to;

using fmt::formatter;

// using fmt::format_error;

}  // namespace xlib

#endif  // defined(_XLIB_LIB_FORMAT)

#endif  // XLIB_UTILITY_FORMAT_FORMAT_HPP_
