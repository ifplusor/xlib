#ifndef XLIB_IO_STREAM_STREAMUTIL_HPP_
#define XLIB_IO_STREAM_STREAMUTIL_HPP_

#include <cstddef>  // ptrdiff_t

#include <algorithm>  // std::min
#include <limits>     // std::numeric_limits

#include "xlib/container/span/span.hpp"
#include "xlib/io/support/io_error.hpp"
#include "xlib/io/support/position.hpp"
#include "xlib/io/support/type.hpp"
#include "xlib/ranges/ranges.hpp"
#include "xlib/types/byte.hpp"

namespace xlib::io::detail {

/**
 * @brief Returns the amount of bytes that can be transferred safely in one operation.
 *
 * @tparam Position Type of the stream position.
 * @param[in] stream_buffer_size Size of the stream buffer.
 * @param[in] stream_position Stream position to start transfer from.
 * @param[in] transfer_buffer_size Size of the buffer to transfer bytes with.
 * @return Amount of bytes to transfer.
 */
template <typename Position>
constexpr auto __get_bytes_to_transfer(Position stream_buffer_size,
                                       Position stream_position,
                                       ptrdiff_t transfer_buffer_size) noexcept {
  // This doesn't overflow and guarantees that position + result fits into the
  // type of position. See 2nd part of the equation. We are already
  // subtracting position so adding it back gives us buffer size which is the
  // same type as position.
  auto temp_result = std::min(transfer_buffer_size, stream_buffer_size - stream_position);
  // Make sure we fit into std::streamsize.
  if constexpr (sizeof(temp_result) > sizeof(streamsize)) {
    temp_result = std::min(temp_result, std::numeric_limits<streamsize>::max());
  }
  // But we still need to make sure that we fit into std::streamoff.
  if constexpr (sizeof(temp_result) <= sizeof(streamoff)) {
    return temp_result;
  }
  // Again, left side doesn't overflow because it was very nicely calculated.
  if (stream_position + temp_result > std::numeric_limits<streamoff>::max()) {
    temp_result = std::numeric_limits<streamoff>::max() - stream_position;
  }
  return temp_result;
}

/**
 * @brief Reads zero or more bytes from the stream to the given buffer and advances stream position by the amount of
 * bytes read.
 *
 * @tparam Buffer Type of the stream buffer.
 * @tparam Position Type of the stream position.
 * @param[in] in_buffer Stream buffer to read from.
 * @param[in,out] pos Stream position to start reading from.
 * @param[in,out] out_buffer Buffer to write to.
 * @return Amount of bytes read.
 * @throw xlib::io::io_error If stream position at the start of reading has maximum value supported by the
 * implementation.
 * @note This function finishes if the given buffer is filled or the end of stream is reached.
 */
template <typename Buffer, typename Position>
constexpr streamsize __read_some(const Buffer& in_buffer, Position& pos, span<byte> out_buffer) {
  auto out_size = ranges::ssize(out_buffer);
  if (out_size == 0) {
    return 0;
  }
  auto in_size = ranges::ssize(in_buffer);
  if (pos >= in_size) {
    // Already at the end of stream.
    return 0;
  }
  if (pos == position::max().value()) {
    // Already at the maximum position supported by implementation.
    throw io_error{"ReadSome", io_errc::value_too_large};
  }
  auto bytes_to_read = __get_bytes_to_transfer(in_size, pos, out_size);
  auto end_position = pos + bytes_to_read;
  auto first = ranges::begin(in_buffer) + pos;
  auto last = ranges::begin(in_buffer) + end_position;
  ranges::copy(first, last, ranges::begin(out_buffer));
  pos = end_position;
  return bytes_to_read;
}

/**
 * @brief Writes zero or more bytes to the fixed-size stream and advances stream position by the amount of bytes
 * written.
 *
 * @tparam Buffer Type of the stream buffer.
 * @tparam Position Type of the stream position.
 * @param[in,out] out_buffer Stream buffer to write to.
 * @param[in,out] pos Stream position to start writing to.
 * @param[in] in_buffer Buffer to read from.
 * @return Amount of bytes written.
 * @throw xlib::io::io_error If stream position at the start of writing is already at the end of the stream buffer or
 * has maximum value supported by the implementation.
 * @note This function finishes if the given buffer is fully written or the end of stream is reached.
 */
template <typename Buffer, typename Position>
constexpr streamsize __write_some_fixed(Buffer& out_buffer, Position& pos, span<const byte> in_buffer) {
  auto in_size = ranges::ssize(in_buffer);
  if (in_size == 0) {
    return 0;
  }
  auto out_size = ranges::ssize(out_buffer);
  if (pos >= out_size) {
    // Already at the end of stream.
    throw io_error{"WriteSomeFixed", io_errc::file_too_large};
  }
  if (pos == position::max().value()) {
    // Already at the maximum position supported by the implementation.
    throw io_error{"WriteSomeFixed", io_errc::file_too_large};
  }
  auto bytes_to_write = __get_bytes_to_transfer(out_size, pos, in_size);
  auto end_position = pos + bytes_to_write;
  auto in_first = ranges::begin(in_buffer);
  auto in_last = in_first + bytes_to_write;
  auto out_first = ranges::begin(out_buffer) + pos;
  ranges::copy(in_first, in_last, out_first);
  pos = end_position;
  return bytes_to_write;
}

/**
 * @brief Writes zero or more bytes to the dynamic-size stream and advances stream position by the amount of bytes
 * written.
 *
 * @tparam Buffer Type of the stream buffer.
 * @tparam Position Type of the stream position.
 * @param[in,out] out_buffer Stream buffer to write to.
 * @param[in,out] pos Stream position to start writing to.
 * @param[in] in_buffer Buffer to read from.
 * @return Amount of bytes written.
 * @throw xlib::io::io_error If stream position at the start of writing is already at the end of the stream buffer of
 * maximum size or has maximum value supported by the implementation.
 * @throw std::bad_alloc If stream position at the start of writing is at the end of the stream buffer and there was not
 * enough memory to allocate the stream buffer of required size.
 */
template <typename Buffer, typename Position>
constexpr streamsize __write_somw_dynamic(Buffer& out_buffer, Position& pos, span<const byte> in_buffer) {
  auto in_size = ranges::ssize(in_buffer);
  if (in_size == 0) {
    return 0;
  }
  auto max_out_size = static_cast<typename Buffer::difference_type>(out_buffer.max_size());
  if (pos >= max_out_size) {
    throw io_error{"WriteSomeDynamic", io_errc::file_too_large};
  }
  if (pos == position::max().value()) {
    // Already at the maximum position supported by the implementation.
    throw io_error{"WriteSomeDynamic", io_errc::file_too_large};
  }
  auto out_size = ranges::ssize(out_buffer);
  if (pos < out_size) {
    // Try to write as much bytes as possible before expanding buffer.
    auto bytes_to_write = __get_bytes_to_transfer(out_size, pos, in_size);
    auto end_position = pos + bytes_to_write;
    auto in_first = ranges::begin(in_buffer);
    auto in_last = in_first + bytes_to_write;
    auto out_first = ranges::begin(out_buffer) + pos;
    ranges::copy(in_first, in_last, out_first);
    pos = end_position;
    return bytes_to_write;
  }
  auto bytes_to_write = __get_bytes_to_transfer(max_out_size, pos, in_size);
  auto end_position = pos + bytes_to_write;
  out_buffer.resize(end_position);
  auto in_first = ranges::begin(in_buffer);
  auto in_last = in_first + bytes_to_write;
  auto out_first = ranges::begin(out_buffer) + pos;
  ranges::copy(in_first, in_last, out_first);
  pos = end_position;
  return bytes_to_write;
}

}  // namespace xlib::io::detail

#endif  // XLIB_IO_STREAM_STREAMUTIL_HPP_
