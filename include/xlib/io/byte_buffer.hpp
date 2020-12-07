#ifndef XLIB_IO_BYTEBUFFER_HPP_
#define XLIB_IO_BYTEBUFFER_HPP_

#include <sstream>  // std::stringstream

#include "xlib/io/buffer.hpp"
#include "xlib/strings/xstring.hpp"
#include "xlib/system/byte_order.hpp"

namespace xlib {

class byte_buffer : public buffer<char> {
 public:
  virtual ~byte_buffer() = default;

  byte_buffer(char* data, std::size_t cap) : byte_buffer(data, cap, cap, 0) {}
  byte_buffer(char* data, std::size_t cap, std::size_t lim, std::size_t pos, std::size_t mark = SIZE_MAX)
      : buffer(cap, lim, pos, mark), data_(data) {}

 public:
  virtual byte_buffer* slice() { return new byte_buffer{data_ + position(), remaining(), remaining(), 0}; }

  virtual byte_buffer* duplicate() { return new byte_buffer{data_, capacity(), limit(), position(), mark_value()}; }

  virtual byte_buffer* as_read_only_buffer() { return nullptr; }

  inline byte_buffer& order(byte_order bo) {
    big_endian_ = (bo == byte_order::big_endian);
    return *this;
  }

  byte_buffer& compact() {
    std::memcpy(data_, data_ + position(), remaining());
    position(remaining());
    limit(capacity());
    discard_mark();
    return *this;
  }

  template <typename T>
  inline T get() {
    return read_bytes<T>(data_ + next_get_index(sizeof(T)), big_endian_);
  }

  template <typename T>
  inline byte_buffer& put(T value) {
    write_bytes(data_ + next_put_index(sizeof(T)), value, big_endian_);
    return *this;
  }

  template <typename T>
  inline T get(std::size_t index) {
    return read_bytes<T>(data_ + check_index(index, sizeof(T)), big_endian_);
  }

  template <typename T>
  inline byte_buffer& put(std::size_t index, T value) {
    write_bytes(data_ + check_index(index, sizeof(T)), value, big_endian_);
    return *this;
  }

  byte_buffer& get(void* dest, std::size_t size, std::size_t offset, std::size_t length) {
    check_bounds(offset, length, size);
    if (length > remaining()) {
      throw std::runtime_error("buffer underflow");
    }
    std::memcpy(static_cast<char*>(dest) + offset, data_ + position(), length);
    position(position() + length);
    return *this;
  }

  byte_buffer& put(const void* src, std::size_t size, std::size_t offset, std::size_t length) {
    if (read_only()) {
      throw std::runtime_error("read-only buffer");
    }
    check_bounds(offset, length, size);
    if (length > remaining()) {
      throw std::runtime_error("buffer overflow");
    }
    std::memcpy(data_ + position(), static_cast<char const*>(src) + offset, length);
    position(position() + length);
    return *this;
  }

 public:
  inline byte_order order() const { return big_endian_ ? byte_order::big_endian : byte_order::little_endian; }

  bool read_only() override { return read_only_; }

  char* array() override final {
    if (data_ == nullptr) {
      throw std::runtime_error("unsupported operation");
    }
    if (read_only()) {
      throw std::runtime_error("read-only buffer");
    }
    return data_;
  }

 protected:
  char* data_;
  bool read_only_ = false;
  bool big_endian_ = true;
};

template <>
inline byte_buffer& byte_buffer::put<xstring>(xstring str) {
  return put(str.data(), str.length(), 0, str.length());
}

template <>
inline char byte_buffer::get<char>() {
  return data_[next_get_index()];
}

template <>
inline byte_buffer& byte_buffer::put<char>(char value) {
  data_[next_put_index()] = value;
  return *this;
}

template <>
inline char byte_buffer::get<char>(std::size_t index) {
  return data_[check_index(index)];
}

template <>
inline byte_buffer& byte_buffer::put<char>(std::size_t index, char value) {
  data_[check_index(index)] = value;
  return *this;
}

}  // namespace xlib

#endif  // XLIB_IO_BYTEBUFFER_HPP_
