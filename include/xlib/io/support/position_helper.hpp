#ifndef XLIB_IO_SUPPORT_POSITIONHELPER_HPP_
#define XLIB_IO_SUPPORT_POSITIONHELPER_HPP_

#include <limits>

#include "xlib/io/support/io_error.hpp"
#include "xlib/io/support/position.hpp"
#include "xlib/ranges/ranges.hpp"

namespace xlib::io::detail {

/**
 * @brief Helper class template for managing stream position.
 * @details
 * This class template manages stream position and provides public member functions that avoid code duplication.
 *
 * @tparam Stream Type of the stream.
 * @tparam Position Type of the stream position.
 */
template <typename Stream, typename Position>
class __position_helper {
 public:
  constexpr __position_helper() noexcept = default;

  [[nodiscard]] constexpr position get_position() const noexcept { return position{position_}; }

  constexpr void seek_position(position pos) {
    auto raw_position = pos.value();
    if (raw_position < 0) {
      throw io_error{"seek_position", io_errc::invalid_argument};
    }
    if (raw_position > std::numeric_limits<Position>::max()) {
      throw io_error{"seek_position", io_errc::value_too_large};
    }
    position_ = raw_position;
  }
  constexpr void seek_position(offset off) { seek_position(move_position(position_, off)); }
  constexpr void seek_position(base_position base) noexcept {
    switch (base) {
      case base_position::beginning: {
        position_ = 0;
      } break;
      case base_position::current:
        break;
      case base_position::end: {
        const auto& buffer = static_cast<Stream*>(this)->get_buffer();
        if constexpr (sizeof(Position) <= sizeof(streamoff)) {
          position_ = ranges::ssize(buffer);
        } else {
          position_ = min(ranges::ssize(buffer), position::max().value());
        }
      } break;
      default:
        break;
    }
  }
  constexpr void seek_position(base_position base, offset off) {
    switch (base) {
      case base_position::beginning: {
        seek_position(position{off});
      } break;
      case base_position::current: {
        seek_position(off);
      } break;
      case base_position::end: {
        const auto& buffer = static_cast<Stream*>(this)->get_buffer();
        seek_position(move_position(ranges::ssize(buffer), off));
      } break;
      default:
        break;
    }
  }

 private:
  /**
   * @brief Moves the stream position by the given offset.
   *
   * @param[in] pos Stream position to move.
   * @param[in] off Offset to move by.
   * @return Moved position.
   * @throw xlib::io::io_error If resulting position is too large.
   */
  static constexpr position move_position(Position pos, offset off) {
    using ResultType = decltype(pos + off.value());
    ResultType temp_position = pos;
    ResultType temp_offset = off.value();
    // Check if adding offset to position overflows.
    // Position can't be negative so no check for underflow here.
    if (temp_offset > std::numeric_limits<ResultType>::max() - temp_position) {
      throw io_error{"MovePosition", io_errc::value_too_large};
    }
    ResultType result = temp_position + temp_offset;
    if constexpr (sizeof(ResultType) == sizeof(streamoff)) {
      return position{result};
    }
    if (result > std::numeric_limits<streamoff>::max()) {
      throw io_error{"MovePosition", io_errc::value_too_large};
    }
    return position{result};
  }

 protected:
  /**
   * @brief Stream position.
   */
  Position position_{0};
};

}  // namespace xlib::io::detail

#endif  // XLIB_IO_SUPPORT_POSITIONHELPER_HPP_
