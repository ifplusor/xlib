#ifndef XLIB_IO_BUFER_BYTEBUFFER_HPP_
#define XLIB_IO_BUFER_BYTEBUFFER_HPP_

#include <cstring>  // std::memcpy

#include "xlib/io/buffer/buffer_manager.hpp"
#include "xlib/io/bytes/bytes_access.hpp"
#include "xlib/numeric/bit/endian.hpp"
#include "xlib/types/byte.hpp"

namespace xlib {

template <typename Derived>
class basic_byte_buffer : public detail::__buffer_manager<Derived> {
  using BaseType = detail::__buffer_manager<Derived>;

 public:
  // Types:
  using size_type = typename BaseType::size_type;

  basic_byte_buffer(byte* data, size_type cap) : BaseType(cap, cap, 0, SIZE_MAX), data_(data) {}
  basic_byte_buffer(byte* data, size_type cap, size_type lim, size_type pos, size_type mark = SIZE_MAX)
      : BaseType(cap, lim, pos, mark), data_(data) {}

  Derived slice() { return {data_ + BaseType::position(), BaseType::remaining(), BaseType::remaining(), 0}; }
  Derived slice(size_type size) {
    if (size > BaseType::remaining()) {
      throw std::runtime_error("buffer overflow");
    }
    return {data_ + BaseType::position(), size, size, 0};
  }

  Derived& order(endian eo) {
    endian_ = eo;
    return *static_cast<Derived*>(this);
  }

  Derived& compact() {
    std::memcpy(data_, data_ + BaseType::position(), BaseType::remaining());
    BaseType::position(BaseType::remaining());
    BaseType::limit(BaseType::capacity());
    BaseType::discard_mark();
    return *static_cast<Derived*>(this);
  }

  template <typename T>
  T read() {
    return xlib::read<T>(data_ + BaseType::next_read_index(sizeof(T)), endian_);
  }
  template <>
  byte read<byte>() {
    return data_[BaseType::next_read_index()];
  }

  template <typename T>
  Derived& write(T value) {
    xlib::write(data_ + BaseType::next_write_index(sizeof(T)), value, endian_);
    return *static_cast<Derived*>(this);
  }
  template <>
  Derived& write<byte>(byte value) {
    data_[BaseType::next_write_index()] = value;
    return *static_cast<Derived*>(this);
  }

  template <typename T>
  T read(size_type index) {
    return xlib::read<T>(data_ + BaseType::check_index(index, sizeof(T)), endian_);
  }
  template <>
  byte read<byte>(size_type index) {
    return data_[BaseType::check_index(index)];
  }

  template <typename T>
  Derived& write(size_type index, T value) {
    xlib::write(data_ + BaseType::check_index(index, sizeof(T)), value, endian_);
    return *static_cast<Derived*>(this);
  }
  template <>
  inline Derived& write<byte>(size_type index, byte value) {
    data_[BaseType::check_index(index)] = value;
    return *static_cast<Derived*>(this);
  }

  Derived& read(byte* dest, size_type size, size_type offset, size_type length) {
    BaseType::check_bounds(offset, length, size);
    if (length > BaseType::remaining()) {
      throw std::runtime_error("buffer underflow");
    }
    std::memcpy(dest + offset, data_ + BaseType::position(), length);
    BaseType::skip(length);
    return *static_cast<Derived*>(this);
  }

  Derived& write(const byte* src, size_type size, size_type offset, size_type length) {
    BaseType::check_bounds(offset, length, size);
    if (length > BaseType::remaining()) {
      throw std::runtime_error("buffer overflow");
    }
    std::memcpy(data_ + BaseType::position(), src + offset, length);
    BaseType::skip(length);
    return *static_cast<Derived*>(this);
  }

  byte* data() { return data_; }
  [[nodiscard]] byte const* data() const { return data_; }

  [[nodiscard]] endian order() const { return endian_; }

 private:
  byte* data_;
  endian endian_{endian::big};
};

class byte_buffer : public basic_byte_buffer<byte_buffer> {
  using base_type = basic_byte_buffer<byte_buffer>;

 public:
  using base_type::basic_byte_buffer;
};

}  // namespace xlib

#endif  // XLIB_IO_BUFER_BYTEBUFFER_HPP_
