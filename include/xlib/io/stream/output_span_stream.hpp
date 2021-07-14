#ifndef XLIB_IO_STREAM_OUTPUTSPANSTREAM_HPP_
#define XLIB_IO_STREAM_OUTPUTSPANSTREAM_HPP_

/*

namespace xlib::io {

class output_span_stream final {
public:
  // Constructors
  constexpr output_span_stream() noexcept = default;
  constexpr output_span_stream(span<byte> buffer) noexcept;

  // Position
  constexpr position get_position() const noexcept;
  constexpr void seek_position(position pos);
  constexpr void seek_position(offset off);
  constexpr void seek_position(base_position base) noexcept;
  constexpr void seek_position(base_position base, offset off);

  // Writing
  constexpr streamsize write_some(span<const byte> in_buffer);

  // Buffer management
  constexpr span<byte> get_buffer() const noexcept;
  constexpr void set_buffer(span<byte> new_buffer) noexcept;
private:
  span<byte> buffer_;  // exposition only
  ptrdiff_t pos_;      // exposition only
};

}  // namespace xlib::io

*/

#include <cstddef>  // ptrdiff_t

#include "xlib/container/span/span.hpp"
#include "xlib/io/support/position_helper.hpp"
#include "xlib/io/support/stream_helper.hpp"
#include "xlib/types/byte.hpp"

namespace xlib::io {

class output_span_stream final : public detail::__position_helper<output_span_stream, ptrdiff_t> {
 public:
  // Constructors
  constexpr output_span_stream() noexcept = default;
  constexpr output_span_stream(span<byte> buffer) noexcept : buffer_{buffer} {};

  // Writing
  constexpr streamsize write_some(span<const byte> buffer) {
    return detail::__write_some_fixed(buffer_, position_, buffer);
  }

  // Buffer management
  [[nodiscard]] constexpr span<byte> get_buffer() const noexcept { return buffer_; };
  constexpr void set_buffer(span<byte> new_buffer) noexcept {
    buffer_ = new_buffer;
    position_ = 0;
  };

 private:
  span<byte> buffer_;
};

}  // namespace xlib::io

#endif  // XLIB_IO_STREAM_OUTPUTSPANSTREAM_HPP_
