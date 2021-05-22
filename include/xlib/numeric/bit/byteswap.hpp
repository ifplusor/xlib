#ifndef XLIB_NUMERIC_BIT_BYTESWAP_HPP_
#define XLIB_NUMERIC_BIT_BYTESWAP_HPP_

#include <cstdint>

#include "xlib/__config.hpp"
#include "xlib/types/type_traits/enable_if.hpp"
#include "xlib/types/type_traits/is_integral.hpp"
#include "xlib/types/type_traits/make_unsigned.hpp"

namespace xlib {

namespace detail {

#if defined(_MSC_VER)

template <typename T>
constexpr void __is_constexpr_helper(T&& t) {}

inline uint64_t __byteswap_impl(uint64_t v) {
  return _byteswap_uint64(v);
}
inline uint32_t __byteswap_impl(uint32_t v) {
  return _byteswap_ulong(v);
}
inline uint16_t __byteswap_impl(uint16_t v) {
  return _byteswap_ushort(v);
}

/**
 * @brief Swaps the upper and lower bytes of a 16-bit integer.
 *
 * @param value
 * @return uint16_t
 */
constexpr uint16_t __byteswap_impl0(uint16_t value) noexcept {
  return static_cast<uint16_t>((value << 8) | (value >> 8));
}

/**
 * @brief Reverses the order of the 4 bytes in a 32-bit integer.
 *
 * @param value
 * @return uint32_t
 */
constexpr uint32_t __byteswap_impl0(uint32_t n) noexcept {
  return (n << 24) | (n >> 24) | ((n & 0x0000ff00) << 8) | ((n & 0x00ff0000) >> 8);
}

/**
 * @brief Reverses the order of the 8 bytes in a 64-bit integer.
 *
 * @param value
 * @return uint64_t
 */
constexpr uint64_t __byteswap_impl0(uint64_t value) noexcept {
  return (static_cast<uint64_t>(__byteswap_impl0(static_cast<uint32_t>(value))) << 32) |
         static_cast<uint64_t>(__byteswap_impl0(static_cast<uint32_t>(value >> 32)));
}

#else  // defined(_MSC_VER)

constexpr uint64_t __byteswap_impl(uint64_t value) noexcept {
  return __builtin_bswap64(value);
}
constexpr uint32_t __byteswap_impl(uint32_t value) noexcept {
  return __builtin_bswap32(value);
}
constexpr uint16_t __byteswap_impl(uint16_t value) noexcept {
  return __builtin_bswap16(value);
}

#endif  // defined(_MSC_VER)

constexpr uint8_t __byteswap_impl(uint8_t value) noexcept {
  return value;
}

}  // namespace detail

template <typename IntegerType, enable_if_t<is_integral<IntegerType>::value, int> = 0>
constexpr IntegerType byteswap(IntegerType value) noexcept {
#if defined(_XLIB_IF_CONSTEXPR)
  if constexpr (sizeof(IntegerType) == 1) {
    return value;
  }
#if defined(_MSC_VER)
  if constexpr (noexcept(detail::__is_constexpr_helper(value))) {
    return detail::__byteswap_impl0(static_cast<make_unsigned_t<IntegerType>>(value));
  }
#endif  // defined(_MSC_VER)
#endif  // defined(_XLIB_IF_CONSTEXPR)
  return detail::__byteswap_impl(static_cast<make_unsigned_t<IntegerType>>(value));
}

}  // namespace xlib

#endif  // XLIB_NUMERIC_BIT_BYTESWAP_HPP_
