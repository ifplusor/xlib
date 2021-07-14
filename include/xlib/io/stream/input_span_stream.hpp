#ifndef XLIB_IO_STREAM_INPUTSPANSTREAM_HPP_
#define XLIB_IO_STREAM_INPUTSPANSTREAM_HPP_

/*

namespace xlib::io {

class input_span_stream final {
public:
  // Constructors
  constexpr input_span_stream() noexcept = default;
  constexpr input_span_stream(span<const byte> buffer) noexcept;

  // Position
  constexpr position get_position() const noexcept;
  constexpr void seek_position(position pos);
  constexpr void seek_position(offset off);
  constexpr void seek_position(base_position base) noexcept;
  constexpr void seek_position(base_position base, offset off);

  // Reading
  constexpr streamsize read_some(span<byte> out_buffer);

  // Buffer management
  constexpr span<const byte> get_buffer() const noexcept;
  constexpr void set_buffer(span<const byte> new_buffer) noexcept;
private:
  span<const byte> buffer_;  // exposition only
  ptrdiff_t pos_;            // exposition only
};

}  // namespace xlib::io

*/

#include <cstddef>  // ptrdiff_t

#include "xlib/container/span/span.hpp"
#include "xlib/io/support/position_helper.hpp"
#include "xlib/io/support/stream_helper.hpp"
#include "xlib/types/byte.hpp"

namespace xlib::io {

class input_span_stream final : public detail::__position_helper<input_span_stream, ptrdiff_t> {
 public:
  constexpr input_span_stream() noexcept = default;
  constexpr input_span_stream(span<const byte> buffer) noexcept : buffer_(buffer){};

  constexpr streamsize read_some(span<byte> buffer) { return detail::__read_some(buffer_, position_, buffer); }

  [[nodiscard]] constexpr span<const byte> get_buffer() const noexcept { return buffer_; }
  constexpr void set_buffer(span<const byte> new_buffer) noexcept {
    buffer_ = new_buffer;
    position_ = 0;
  }

 private:
  span<const byte> buffer_;
};

}  // namespace xlib::io

#endif  // XLIB_IO_STREAM_INPUTSPANSTREAM_HPP_