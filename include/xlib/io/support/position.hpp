#ifndef XLIB_IO_SUPPORT_POSITION_HPP_
#define XLIB_IO_SUPPORT_POSITION_HPP_

/*

namespace xlib::io {

class position final {
public:
  // Constructors
  constexpr position() = default;
  constexpr explicit position(streamoff pos) noexcept;
  constexpr explicit position(offset off) noexcept;

  // Observer
  constexpr streamoff value() const noexcept;

  // Arithmetic
  constexpr position& operator++() noexcept;
  constexpr position operator++(int) noexcept;
  constexpr position& operator--() noexcept;
  constexpr position operator--(int) noexcept;

  constexpr position& operator+=(offset rhs) noexcept;
  constexpr position& operator-=(offset rhs) noexcept;

  // Special values
  static constexpr position max() noexcept;
private:
  streamoff position_;  // exposition only
};

}  // namespace xlib::io

*/

#include <limits>

#include "xlib/__config.hpp"
#include "xlib/io/support/offset.hpp"

#if defined(_XLIB_LIB_THREE_WAY_COMPARISON)

#include <compare>  // std::strong_ordering;

#endif  // defined(_XLIB_LIB_THREE_WAY_COMPARISON)

namespace xlib::io {

class position final {
 public:
  _XLIB_CONSTEXPR_CTOR position() = default;
  constexpr explicit position(streamoff pos) noexcept : position_(pos) {}
  constexpr explicit position(offset off) noexcept : position_(off.value()) {}

  [[nodiscard]] constexpr streamoff value() const noexcept { return position_; }

  constexpr position& operator++() noexcept {
    ++position_;
    return *this;
  }
  constexpr position operator++(int) noexcept { return position{position_++}; }

  constexpr position& operator--() noexcept {
    --position_;
    return *this;
  }
  constexpr position operator--(int) noexcept { return position{position_--}; }

  constexpr position& operator+=(offset rhs) noexcept {
    position_ += rhs.value();
    return *this;
  }
  constexpr position& operator-=(offset rhs) noexcept {
    position_ -= rhs.value();
    return *this;
  }

  static constexpr position max() noexcept { return position{std::numeric_limits<streamoff>::max()}; }

  friend constexpr bool operator==(position lhs, position rhs) noexcept { return lhs.position_ == rhs.position_; }

#if defined(_XLIB_LIB_THREE_WAY_COMPARISON)
  friend constexpr std::strong_ordering operator<=>(position lhs, position rhs) noexcept {
    return lhs.position_ <=> rhs.position_;
  }
#endif  // defined(_XLIB_LIB_THREE_WAY_COMPARISON)

  // FIXME
  friend constexpr position operator+(position lhs, offset rhs) noexcept {
    return position{lhs.position_ + rhs.value()};
  }
  friend constexpr position operator+(offset lhs, position rhs) noexcept {
    return position{lhs.value() + rhs.position_};
  }
  friend constexpr position operator-(position lhs, offset rhs) noexcept {
    return position{lhs.position_ - rhs.value()};
  }
  friend constexpr offset operator-(position lhs, position rhs) noexcept {
    return offset{lhs.position_ - rhs.position_};
  }

 private:
  streamoff position_;
};

enum class base_position { beginning, current, end };

}  // namespace xlib::io

#endif  // XLIB_IO_SUPPORT_POSITION_HPP_
