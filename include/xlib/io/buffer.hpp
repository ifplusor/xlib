#ifndef XLIB_IO_BUFFER_HPP_
#define XLIB_IO_BUFFER_HPP_

#include <cstddef>
#include <cstdint>

#include <stdexcept>  // std::invalid_argument, std::runtime_error
#include <string>

namespace xlib {

template <typename DataT>
class buffer {
 public:
  using value_type = DataT;

 public:
  ~buffer() = default;

  virtual value_type* array() = 0;
  virtual bool read_only() = 0;

 protected:
  buffer(std::size_t cap, std::size_t lim, std::size_t pos, std::size_t mark = SIZE_MAX) {
    capacity_ = cap;
    limit(lim);
    position(pos);
    if (mark != SIZE_MAX) {
      if (mark > pos) {
        throw std::invalid_argument("mark > position: (" + std::to_string(mark) + " > " + std::to_string(pos) + ")");
      }
      mark_ = mark;
    }
  }

 public:
  inline buffer& position(std::size_t new_position) {
    if (new_position > limit_) {
      throw std::invalid_argument("index out of bounds");
    }
    position_ = new_position;
    if (mark_ > position_) {
      mark_ = SIZE_MAX;
    }
    return *this;
  }

  inline buffer& limit(std::size_t new_limit) {
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
    return *this;
  }

  inline buffer& mark() {
    mark_ = position_;
    return *this;
  }

  inline buffer& reset() {
    std::size_t m = mark_;
    if (m == SIZE_MAX) {
      throw std::runtime_error("invalid mark");
    }
    position_ = m;
    return *this;
  }

  inline buffer& clear() {
    position_ = 0;
    limit_ = capacity_;
    mark_ = SIZE_MAX;
    return *this;
  }

  inline buffer& flip() {
    limit_ = position_;
    position_ = 0;
    mark_ = SIZE_MAX;
    return *this;
  }

  inline buffer& rewind() {
    position_ = 0;
    mark_ = SIZE_MAX;
    return *this;
  }

  inline std::size_t remaining() const { return limit_ - position_; }
  inline bool has_remaining() const { return position_ < limit_; }

 protected:
  inline std::size_t next_get_index() {
    if (position_ >= limit_) {
      throw std::runtime_error("buffer underflow");
    }
    return position_++;
  }

  inline std::size_t next_get_index(std::size_t nb) {
    if (limit_ - position_ < nb) {
      throw std::runtime_error("buffer underflow");
    }
    std::size_t p = position_;
    position_ += nb;
    return p;
  }

  inline std::size_t next_put_index() {
    if (position_ >= limit_) {
      throw std::runtime_error("buffer overflow");
    }
    return position_++;
  }

  inline std::size_t next_put_index(std::size_t nb) {
    if (limit_ - position_ < nb) {
      throw std::runtime_error("buffer overflow");
    }
    std::size_t p = position_;
    position_ += nb;
    return p;
  }

  inline std::size_t check_index(std::size_t i) const {
    if ((i < 0) || (i >= limit_)) {
      throw std::runtime_error("index out of bounds");
    }
    return i;
  }

  inline std::size_t check_index(std::size_t i, std::size_t nb) const {
    if ((i < 0) || (nb > limit_ - i)) {
      throw std::runtime_error("index out of bounds");
    }
    return i;
  }

  inline void truncate() {
    mark_ = SIZE_MAX;
    position_ = 0;
    limit_ = 0;
    capacity_ = 0;
  }

  inline void discard_mark() { mark_ = SIZE_MAX; }

  static inline void check_bounds(std::size_t off, std::size_t len, std::size_t size) {
    if (off + len > size) {
      throw std::runtime_error("index out of bounds");
    }
  }

 public:
  inline std::size_t mark_value() const { return mark_; }
  inline std::size_t position() const { return position_; }
  inline std::size_t limit() const { return limit_; }
  inline std::size_t capacity() const { return capacity_; }

 private:
  // Invariants: mark <= position <= limit <= capacity
  std::size_t mark_ = SIZE_MAX;
  std::size_t position_ = 0;
  std::size_t limit_;
  std::size_t capacity_;
};

}  // namespace xlib

#endif  // XLIB_IO_BUFFER_HPP_
