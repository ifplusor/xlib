#ifndef XLIB_IO_SUPPORT_IOERRC_HPP_
#define XLIB_IO_SUPPORT_IOERRC_HPP_

#include <cerrno>

#include "xlib/error/error_code.hpp"
#include "xlib/types/type_traits/integral_constant.hpp"

namespace xlib::io {

enum class io_errc {
  bad_file_descriptor = EBADF,
  invalid_argument = EINVAL,
  value_too_large = EOVERFLOW,
  reached_end_of_file,
  interrupted = EINTR,
  physical_error = EIO,
  file_too_large = EFBIG
};

}  // namespace xlib::io

namespace std {

template <>
struct is_error_code_enum<xlib::io::io_errc> : public true_type {};

}  // namespace std

#endif  // XLIB_IO_SUPPORT_IOERRC_HPP_
