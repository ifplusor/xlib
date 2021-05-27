#ifndef XLIB_FILESYSTEM_FILESYSTEM_HPP_
#define XLIB_FILESYSTEM_FILESYSTEM_HPP_

#include "xlib/__config.hpp"

#if defined(_XLIB_LIB_RANGES)

#include <ranges>

namespace xlib {

namespace ranges = std::ranges;

}  // namespace xlib

#else  // defined(_XLIB_LIB_FILESYSTEM)

#include <utility>  // std::declval, std::forward

#include <range/v3/all.hpp>

#include "xlib/types/type_traits/make_signed.hpp"

namespace xlib::ranges {

using namespace ::ranges;

template <typename R>
make_signed_t<decltype(::ranges::size(std::declval<R>()))> ssize(R&& r) {
  using return_type = make_signed_t<decltype(::ranges::size(std::declval<R>()))>;
  return static_cast<return_type>(size(std::forward(r)));
}

}  // namespace xlib::ranges

#endif  // defined(_XLIB_LIB_FILESYSTEM)

#endif  // XLIB_FILESYSTEM_FILESYSTEM_HPP_
