#ifndef XLIB_IO_SUPPORT_OFFSET_HPP_
#define XLIB_IO_SUPPORT_OFFSET_HPP_

/*

namespace xlib::io {

class offset final {
public:
  // Constructors
  constexpr offset() = default;
  constexpr explicit offset(streamoff off) noexcept;

  // Observer
  constexpr streamoff value() const noexcept;

  // Arithmetic
  constexpr offset operator+() const noexcept;
  constexpr offset operator-() const noexcept;
  constexpr offset& operator++() noexcept;
  constexpr offset operator++(int) noexcept;
  constexpr offset& operator--() noexcept;
  constexpr offset operator--(int) noexcept;

  constexpr offset& operator+=(offset rhs) noexcept;
  constexpr offset& operator-=(offset rhs) noexcept;

  constexpr offset& operator*=(streamoff rhs) noexcept;
  constexpr offset& operator/=(streamoff rhs) noexcept;
  constexpr offset& operator%=(streamoff rhs) noexcept;
private:
  streamoff offset_; // exposition only
};

constexpr bool operator==(offset lhs, offset rhs) noexcept;
constexpr strong_ordering operator<=>(offset lhs, offset rhs) noexcept;

constexpr offset operator+(offset lhs, offset rhs) noexcept;
constexpr offset operator-(offset lhs, offset rhs) noexcept;
constexpr offset operator*(offset lhs, streamoff rhs) noexcept;
constexpr offset operator*(streamoff lhs, offset rhs) noexcept;
constexpr offset operator/(offset lhs, streamoff rhs) noexcept;
constexpr offset operator%(offset lhs, streamoff rhs) noexcept;

}  // namespace xlib::io

*/

#include "xlib/__config.hpp"
#include "xlib/io/support/type.hpp"

#if defined(_XLIB_LIB_THREE_WAY_COMPARISON)

#include <compare>  // std::strong_ordering;

#endif  // defined(_XLIB_LIB_THREE_WAY_COMPARISON)

namespace xlib::io {

class offset final {
 public:
  _XLIB_CONSTEXPR_CTOR offset() = default;
  constexpr explicit offset(streamoff off) noexcept : offset_(off) {}

  [[nodiscard]] constexpr streamoff value() const noexcept { return offset_; }

  constexpr offset operator+() const noexcept { return *this; }
  constexpr offset operator-() const noexcept { return offset{-offset_}; }

  constexpr offset& operator++() noexcept {
    ++offset_;
    return *this;
  }
  constexpr offset operator++(int) noexcept { return offset{offset_++}; }

  constexpr offset& operator--() noexcept {
    --offset_;
    return *this;
  }
  constexpr offset operator--(int) noexcept { return offset{offset_--}; }

  constexpr offset& operator+=(offset rhs) noexcept {
    offset_ += rhs.offset_;
    return *this;
  }
  constexpr offset& operator-=(offset rhs) noexcept {
    offset_ -= rhs.offset_;
    return *this;
  }

  constexpr offset& operator*=(streamoff rhs) noexcept {
    offset_ *= rhs;
    return *this;
  }
  constexpr offset& operator/=(streamoff rhs) noexcept {
    offset_ /= rhs;
    return *this;
  }
  constexpr offset& operator%=(streamoff rhs) noexcept {
    offset_ %= rhs;
    return *this;
  }

  friend constexpr bool operator==(offset lhs, offset rhs) noexcept { return lhs.offset_ == rhs.offset_; }

#if defined(_XLIB_LIB_THREE_WAY_COMPARISON)
  friend constexpr std::strong_ordering operator<=>(offset lhs, offset rhs) noexcept {
    return lhs.offset_ <=> rhs.offset_;
  }
#endif  // defined(_XLIB_LIB_THREE_WAY_COMPARISON)

  // FIXME
  friend constexpr offset operator+(offset lhs, offset rhs) noexcept { return offset{lhs.offset_ += rhs.offset_}; }
  friend constexpr offset operator-(offset lhs, offset rhs) noexcept { return offset{lhs.offset_ - rhs.offset_}; }
  friend constexpr offset operator*(offset lhs, streamoff rhs) noexcept { return offset{lhs.offset_ * rhs}; }
  friend constexpr offset operator*(streamoff lhs, offset rhs) noexcept { return offset{lhs * rhs.offset_}; }
  friend constexpr offset operator/(offset lhs, streamoff rhs) noexcept { return offset{lhs.offset_ / rhs}; }
  friend constexpr offset operator%(offset lhs, streamoff rhs) noexcept { return offset{lhs.offset_ % rhs}; }

 private:
  streamoff offset_;
};

}  // namespace xlib::io

#endif  // XLIB_IO_SUPPORT_OFFSET_HPP_
