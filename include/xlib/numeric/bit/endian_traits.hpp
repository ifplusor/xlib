#ifndef XLIB_NUMERIC_BIT_ENDIANTRAITS_HPP_
#define XLIB_NUMERIC_BIT_ENDIANTRAITS_HPP_

#include "xlib/numeric/bit/endian.hpp"

namespace xlib::detail {

struct big_endian_t {};
_XLIB_INLINE_VAR constexpr big_endian_t big_endian{};

struct little_endian_t {};
_XLIB_INLINE_VAR constexpr little_endian_t little_endian{};

struct unknown_endian_t {};
_XLIB_INLINE_VAR constexpr unknown_endian_t unknown_endian{};

template <endian Endian = endian::native>
struct endian_traits {
  using endian_category = unknown_endian_t;
};

template <>
struct endian_traits<endian::big> {
  using endian_category = big_endian_t;
};

template <>
struct endian_traits<endian::little> {
  using endian_category = little_endian_t;
};

}  // namespace xlib::detail

#endif  // XLIB_NUMERIC_BIT_ENDIANTRAITS_HPP_
