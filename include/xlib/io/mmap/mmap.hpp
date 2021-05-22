#ifndef XLIB_IO_MMAP_MMAP_HPP_
#define XLIB_IO_MMAP_MMAP_HPP_

#include <mio/mmap.hpp>

#include "xlib/types/byte.hpp"

namespace xlib::mmap {

using mmap_source = mio::basic_mmap_source<byte>;
using mmap_sink = mio::basic_mmap_sink<byte>;

template <typename MappingToken>
mmap_source make_mmap_source(const MappingToken& token,
                             mmap_source::size_type offset,
                             mmap_source::size_type length,
                             std::error_code& error) {
  return mio::make_mmap<mmap_source>(token, offset, length, error);
}

template <typename MappingToken>
mmap_source make_mmap_source(const MappingToken& token, std::error_code& error) {
  return make_mmap_source(token, 0, mio::map_entire_file, error);
}

template <typename MappingToken>
mmap_sink make_mmap_sink(const MappingToken& token,
                         mmap_sink::size_type offset,
                         mmap_sink::size_type length,
                         std::error_code& error) {
  return mio::make_mmap<mmap_sink>(token, offset, length, error);
}

template <typename MappingToken>
mmap_sink make_mmap_sink(const MappingToken& token, std::error_code& error) {
  return make_mmap_sink(token, 0, mio::map_entire_file, error);
}

}  // namespace xlib::mmap

#endif  // XLIB_IO_MMAP_MMAP_HPP_
