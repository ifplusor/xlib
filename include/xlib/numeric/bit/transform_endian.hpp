/**
 * @file transform_endian.hpp
 * @brief Contains static methods for converting the byte order between different endiannesses.
 */
#ifndef XLIB_NUMERIC_BIT_ENDIANCONVERT_HPP_
#define XLIB_NUMERIC_BIT_ENDIANCONVERT_HPP_

#include <cstdint>  // uint8_t, uint16_t, uint32_t, uint64_t

#include "xlib/__config.hpp"
#include "xlib/numeric/bit/byteswap.hpp"
#include "xlib/types/type_traits/always_false.hpp"
#include "xlib/types/type_traits/enable_if.hpp"
#include "xlib/types/type_traits/is_integral.hpp"
#include "xlib/types/type_traits/make_unsigned.hpp"

#if defined(_XLIB_IF_CONSTEXPR)

#include "xlib/numeric/bit/endian.hpp"

namespace xlib {

/**
 * @brief Transform integer for little-endian
 *
 * @tparam T
 * @param value
 * @return make_unsigned_t<T>
 */
template <typename T, enable_if_t<is_integral<T>::value, int> = 0>
constexpr T transform_little_endian(T value) {
  if constexpr (endian::native == endian::big) {
    return byteswap(value);
  } else if constexpr (endian::native == endian::little) {
    return value;
  } else {
    static_assert(always_false_v<T>, "not support mixed endian platform");
  }
}

/**
 * @brief Transform integer for big-endian
 *
 * @tparam T
 * @param value
 * @return make_unsigned_t<T>
 */
template <typename T, enable_if_t<is_integral<T>::value, int> = 0>
constexpr T transform_big_endian(T value) {
  if constexpr (endian::native == endian::little) {
    return byteswap(value);
  } else if constexpr (endian::native == endian::big) {
    return value;
  } else {
    static_assert(always_false_v<T>, "not support mixed endian platform");
  }
}

}  // namespace xlib

#else  // defined(_XLIB_IF_CONSTEXPR)
/** Use tag dispatch if no constexpr if */

#include "xlib/numeric/bit/endian_traits.hpp"

namespace xlib {

namespace detail {

template <typename T>
constexpr T __transform_little_endian_impl(T value, big_endian_t /*unused*/) {
  return byteswap(value);
}

template <typename T>
constexpr T __transform_little_endian_impl(T value, little_endian_t /*unused*/) {
  return value;
}

template <typename T>
constexpr T __transform_little_endian_impl(T value, unknown_endian_t /*unused*/) {
  static_assert(always_false<T>::value, "not support mixed endian platform");
}

template <typename T>
constexpr T __transform_big_endian_impl(T value, big_endian_t /*unused*/) {
  return byteswap(value);
}

template <typename T>
constexpr T __transform_big_endian_impl(T value, little_endian_t /*unused*/) {
  return __swap_endian(static_cast<make_unsigned_t<T>>(value));
}

template <typename T>
constexpr T __transform_big_endian_impl(T value, unknown_endian_t /*unused*/) {
  static_assert(always_false<T>::value, "not support mixed endian platform");
}

}  // namespace detail

/**
 * @brief Transform integer for little-endian
 *
 * @tparam T
 * @param value
 * @return make_unsigned_t<T>
 */
template <typename T, enable_if_t<is_integral<T>::value, int> = 0>
constexpr T transform_little_endian(T value) {
  return detail::__transform_little_endian_impl(value, detail::endian_traits<>::endian_category());
}

/**
 * @brief Transform integer for big-endian
 *
 * @tparam T
 * @param value
 * @return make_unsigned_t<T>
 */
template <typename T, enable_if_t<is_integral<T>::value, int> = 0>
constexpr T transform_big_endian(T value) {
  return detail::__transform_big_endian_impl(value, detail::endian_traits<>::endian_category());
}

}  // namespace xlib

#endif  // defined(_XLIB_IF_CONSTEXPR)

#endif  // XLIB_NUMERIC_BIT_ENDIANCONVERT_HPP_
