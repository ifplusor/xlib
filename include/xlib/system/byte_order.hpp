/**
 * Contains static methods for converting the byte order between different endiannesses.
 */
#ifndef XLIB_SYSTEM_BYTEORDER_H_
#define XLIB_SYSTEM_BYTEORDER_H_

#include <cstdint>  // uint8_t, uint16_t, uint32_t, uint64_t
#include <cstring>  // std::memcpy

#include <type_traits>  // std::enable_if, std::is_integral, std::make_unsigned, std::add_pointer

namespace xlib {

enum class byte_order { big_endian, little_endian };

//==============================================================================

constexpr byte_order native_byte_order() {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__  // __BYTE_ORDER__ is defined by GCC
  return byte_order::little_endian;
#else
  return byte_order::big_endian;
#endif
}

//==============================================================================

static inline uint8_t swap_byte_order(uint8_t n) {
  return n;
}

/** Swaps the upper and lower bytes of a 16-bit integer. */
static inline uint16_t swap_byte_order(uint16_t n) {
  return static_cast<uint16_t>((n << 8) | (n >> 8));
}

/** Reverses the order of the 4 bytes in a 32-bit integer. */
static inline uint32_t swap_byte_order(uint32_t n) {
  return (n << 24) | (n >> 24) | ((n & 0x0000ff00) << 8) | ((n & 0x00ff0000) >> 8);
}

/** Reverses the order of the 8 bytes in a 64-bit integer. */
static inline uint64_t swap_byte_order(uint64_t value) {
  return (((uint64_t)swap_byte_order((uint32_t)value)) << 32) | swap_byte_order((uint32_t)(value >> 32));
}

//==============================================================================

/** convert integer to little-endian */
template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
inline typename std::make_unsigned<T>::type to_little_endian(T value) {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  return swap_byte_order(static_cast<typename std::make_unsigned<T>::type>(value));
#else
  return static_cast<typename std::make_unsigned<T>::type>(value);
#endif
}

/** convert integer to big-endian */
template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
inline typename std::make_unsigned<T>::type to_big_endian(T value) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  return swap_byte_order(static_cast<typename std::make_unsigned<T>::type>(value));
#else
  return static_cast<typename std::make_unsigned<T>::type>(value);
#endif
}

//==============================================================================

template <typename T, int Enable = 0>
inline T read_bytes(const char* bytes) {
  T value;
  std::memcpy(&value, bytes, sizeof(T));
  return value;
}

template <typename T, typename std::enable_if<sizeof(T) <= 8, int>::value = 0>
inline T read_bytes(const char* bytes) {
  T value;
  for (size_t i = 0; i < sizeof(T); i++) {
    ((char*)&value)[i] = bytes[i];
  }
  return value;
}

template <typename T, int Enable = 0>
inline void read_bytes(T* value, const char* bytes) {
  std::memcpy(value, bytes, sizeof(T));
}

template <typename T, typename std::enable_if<sizeof(T) <= 8, int>::value = 0>
inline void read_bytes(T* value, const char* bytes) {
  for (size_t i = 0; i < sizeof(T); i++) {
    ((char*)value)[i] = bytes[i];
  }
}

//==============================================================================

template <typename T, int Enable = 0>
inline void write_bytes(char* bytes, T value) {
  std::memcpy(bytes, &value, sizeof(T));
}

template <typename T, typename std::enable_if<sizeof(T) <= 8, int>::value = 0>
inline void write_bytes(char* bytes, T value) {
  for (size_t i = 0; i < sizeof(T); i++) {
    bytes[i] = ((char*)&value)[i];
  }
}

//==============================================================================

template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
inline T read_little_endian(const char* bytes) {
  auto value = read_bytes<T>(bytes);
  return to_little_endian(value);
}

template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
inline T read_big_endian(const char* bytes) {
  auto value = read_bytes<T>(bytes);
  return to_big_endian(value);
}

template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
inline T read_bytes(const char* bytes, bool big_endian) {
  return big_endian ? read_big_endian<T>(bytes) : read_little_endian<T>(bytes);
}

//==============================================================================

template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
inline void write_little_endian(char* bytes, T value) {
  value = to_little_endian(value);
  write_bytes(bytes, value);
}

template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
inline void write_big_endian(char* bytes, T value) {
  value = to_big_endian(value);
  write_bytes(bytes, value);
}

template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
inline void write_bytes(char* bytes, T value, bool big_endian) {
  if (big_endian) {
    write_big_endian(bytes, value);
  } else {
    write_little_endian(bytes, value);
  }
}

}  // namespace xlib

#endif  // XLIB_SYSTEM_BYTEORDER_H_
