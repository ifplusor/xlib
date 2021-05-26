#ifndef XLIB_CONTAINER_SPAN_BYTESPAN_HPP_
#define XLIB_CONTAINER_SPAN_BYTESPAN_HPP_

#include "xlib/__config.hpp"

#if _XLIB_STD_VER >= 2020L

#include "xlib/container/span/span.hpp"
#include "xlib/types/byte.hpp"

namespace xlib {

template <class T>
[[nodiscard]] constexpr span<byte> byte_span(T& t) noexcept {
  return span<byte>(reinterpret_cast<byte*>(&t), sizeof(T));
}

template <class T>
[[nodiscard]] constexpr span<const byte> byte_span(T const& t) noexcept {
  return span<const byte>(reinterpret_cast<byte const*>(&t), sizeof(T));
}

}  // namespace xlib

#else  // _XLIB_STD_VER >= 2020L

#include <gsl-lite/gsl-lite.hpp>

namespace xlib {

using gsl::byte_span;

}  // namespace xlib

#endif  // _XLIB_STD_VER >= 2020L

#endif  // XLIB_CONTAINER_SPAN_BYTESPAN_HPP_
