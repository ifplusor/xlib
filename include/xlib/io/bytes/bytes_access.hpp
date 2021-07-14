#ifndef XLIB_IO_BYTES_BYTESACCESS_HPP_
#define XLIB_IO_BYTES_BYTESACCESS_HPP_

#include <cstring>  // std::memcpy

#include "xlib/__config.hpp"
#include "xlib/container/span/byte_span.hpp"
#include "xlib/container/span/span.hpp"
#include "xlib/numeric/bit/transform_endian.hpp"
#include "xlib/types/byte.hpp"
#include "xlib/types/type_traits/enable_if.hpp"
#include "xlib/types/type_traits/is_integral.hpp"

namespace xlib {

//==============================================================================

template <typename T, enable_if_t<!is_integral<T>::value, int> = 0>
void read(const byte* bytes, T* value) {
  std::memcpy(value, bytes, sizeof(T));
}

template <typename T, enable_if_t<is_integral<T>::value, int> = 0>
void read(const byte* bytes, T* value) {
  span<const byte> src_span{bytes, sizeof(T)};
  auto dest_span = byte_span(value);
  for (size_t i = 0; i < src_span.size(); ++i) {
    dest_span[i] = src_span[i];
  }
}

template <typename T, enable_if_t<is_integral<T>::value, int> = 0>
T read(const byte* bytes) {
  T value;
  read(bytes, &value);
  return value;
}

//==============================================================================

template <typename T, enable_if_t<!is_integral<T>::value, int> = 0>
void write(byte* bytes, const T& value) {
  std::memcpy(bytes, &value, sizeof(T));
}

template <typename T, enable_if_t<is_integral<T>::value, int> = 0>
void write(byte* bytes, const T& value) {
  auto src_span = byte_span(value);
  span<byte> dest_span{bytes, sizeof(T)};
  for (size_t i = 0; i < src_span.size(); ++i) {
    dest_span[i] = src_span[i];
  }
}

//==============================================================================

template <typename T, enable_if_t<is_integral<T>::value, int> = 0>
T read_little_endian(const byte* bytes) {
  auto value = read<T>(bytes);
  return transform_little_endian(value);
}

template <typename T, enable_if_t<is_integral<T>::value, int> = 0>
T read_big_endian(const byte* bytes) {
  auto value = read<T>(bytes);
  return transform_big_endian(value);
}

template <typename T, enable_if_t<is_integral<T>::value, int> = 0>
T read(const byte* bytes, endian source_endian) {
  return source_endian == endian::big ? read_big_endian<T>(bytes) : read_little_endian<T>(bytes);
}

//==============================================================================

template <typename T, enable_if_t<is_integral<T>::value, int> = 0>
void write_little_endian(byte* bytes, T value) {
  value = transform_little_endian(value);
  write(bytes, value);
}

template <typename T, enable_if_t<is_integral<T>::value, int> = 0>
void write_big_endian(byte* bytes, T value) {
  value = transform_big_endian(value);
  write(bytes, value);
}

template <typename T, enable_if_t<is_integral<T>::value, int> = 0>
void write(byte* bytes, T value, endian target_endian) {
  if (target_endian == endian::big) {
    write_big_endian(bytes, value);
  } else {
    write_little_endian(bytes, value);
  }
}

}  // namespace xlib

#endif  // XLIB_IO_BYTES_BYTESACCESS_HPP_
