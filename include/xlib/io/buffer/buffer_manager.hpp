#ifndef XLIB_IO_BUFFER_BUFFERMANAGER_HPP_
#define XLIB_IO_BUFFER_BUFFERMANAGER_HPP_

#include <cstddef>  // size_t
#include <cstdint>  // SIZE_MAX

#include <stdexcept>  // std::invalid_argument, std::runtime_error
#include <string>     // std::to_string

namespace xlib::detail {

template <typename Derived>
class __buffer_manager {
 public:
  // Types:
  using size_type = size_t;

  __buffer_manager(size_type cap, size_type lim, size_type pos, size_type mark = SIZE_MAX)
      : capacity_(cap), limit_(cap) {
    limit(lim);
    position(pos);
    if (mark != SIZE_MAX) {
      if (mark > pos) {
        throw std::invalid_argument("mark > position: (" + std::to_string(mark) + " > " + std::to_string(pos) + ")");
      }
      mark_ = mark;
    }
  }

  Derived& position(size_type new_position) {
    if (new_position != position_) {
      if (new_position > limit_) {
        throw std::invalid_argument("index out of bounds");
      }
      position_ = new_position;
      if (mark_ > position_) {
        mark_ = SIZE_MAX;
      }
    }
    return *static_cast<Derived*>(this);
  }

  Derived& skip(size_type length) { return position(position() + length); }

  Derived& limit(size_type new_limit) {
    if (new_limit > capacity_) {
      throw std::invalid_argument("index out of bounds");
    }
    limit_ = new_limit;
    if (position_ > limit_) {
      position_ = limit_;
    }
    if (mark_ > limit_) {
      mark_ = SIZE_MAX;
    }
    return *static_cast<Derived*>(this);
  }

  Derived& mark() {
    mark_ = position_;
    return *static_cast<Derived*>(this);
  }

  Derived& reset() {
    size_type m = mark_;
    if (m == SIZE_MAX) {
      throw std::runtime_error("invalid mark");
    }
    position_ = m;
    return *static_cast<Derived*>(this);
  }

  Derived& clear() {
    position_ = 0;
    limit_ = capacity_;
    mark_ = SIZE_MAX;
    return *static_cast<Derived*>(this);
  }

  Derived& flip() {
    limit_ = position_;
    position_ = 0;
    mark_ = SIZE_MAX;
    return *static_cast<Derived*>(this);
  }

  Derived& rewind() {
    position_ = 0;
    mark_ = SIZE_MAX;
    return *static_cast<Derived*>(this);
  }

  [[nodiscard]] size_type remaining() const { return limit_ - position_; }
  [[nodiscard]] bool has_remaining() const { return position_ < limit_; }

  [[nodiscard]] size_type mark_value() const { return mark_; }
  [[nodiscard]] size_type position() const { return position_; }
  [[nodiscard]] size_type limit() const { return limit_; }
  [[nodiscard]] size_type capacity() const { return capacity_; }

 protected:
  size_type next_read_index() {
    if (position_ >= limit_) {
      throw std::runtime_error("buffer underflow");
    }
    return position_++;
  }

  size_type next_read_index(size_type length) {
    if (limit_ - position_ < length) {
      throw std::runtime_error("buffer underflow");
    }
    size_type p = position_;
    position_ += length;
    return p;
  }

  size_type next_write_index() {
    if (position_ >= limit_) {
      throw std::runtime_error("buffer overflow");
    }
    return position_++;
  }

  size_type next_write_index(size_type length) {
    if (limit_ - position_ < length) {
      throw std::runtime_error("buffer overflow");
    }
    size_type p = position_;
    position_ += length;
    return p;
  }

  [[nodiscard]] size_type check_index(size_type index) const {
    if ((index < 0) || (index >= limit_)) {
      throw std::runtime_error("index out of bounds");
    }
    return index;
  }

  [[nodiscard]] size_type check_index(size_type index, size_type length) const {
    if ((index < 0) || (length > limit_ - index)) {
      throw std::runtime_error("index out of bounds");
    }
    return index;
  }

  void truncate() {
    mark_ = SIZE_MAX;
    position_ = 0;
    limit_ = 0;
    capacity_ = 0;
  }

  void discard_mark() { mark_ = SIZE_MAX; }

  static void check_bounds(size_type offset, size_type length, size_type size) {
    if (offset + length > size) {
      throw std::runtime_error("index out of bounds");
    }
  }

 private:
  // Invariants: mark <= position <= limit <= capacity
  size_type mark_{SIZE_MAX};
  size_type position_{0};
  size_type limit_;
  size_type capacity_;
};

}  // namespace xlib::detail

#endif  // XLIB_IO_BUFFER_BUFFERMANAGER_HPP_
