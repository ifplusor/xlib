#ifndef XLIB_NUMERIC_BIT_BITCAST_HPP_
#define XLIB_NUMERIC_BIT_BITCAST_HPP_

#include "xlib/__config.hpp"

#if defined(_XLIB_LIB_BIT_CAST)

#include <bit>

namespace xlib {

using std::bit_cast;

}  // namespace xlib

#else  // defined(_XLIB_LIB_BIT_CAST)

#include <cstring>  // std::memcpy

#include "xlib/concepts/concept.hpp"
#include "xlib/types/type_traits/is_trivially_copyable.hpp"

namespace xlib {

namespace detail {

template <typename To, typename From>
xlib_concept __can_bit_cast =
    sizeof(To) == sizeof(From) && is_trivially_copyable<To>::value&& is_trivially_copyable<From>::value;

}  // namespace detail

template <class To, class From>
xlib_requires_st(inline constexpr, (detail::__can_bit_cast<To, From>), To)
bit_cast(const From& from) noexcept {
  typename std::aligned_storage<sizeof(To), alignof(To)>::type storage;
  std::memcpy(&storage, &from, sizeof(To));  // Above `constexpr` is optimistic, fails here.
  return reinterpret_cast<To&>(storage);
  // More common implementation:
  // std::remove_const_t<To> to{};
  // std::memcpy(&to, &from, sizeof(To));  // Above `constexpr` is optimistic, fails here.
  // return to;
}

}  // namespace xlib

#endif  // defined(_XLIB_LIB_BIT_CAST)

#endif  // XLIB_NUMERIC_BIT_BITCAST_HPP_
