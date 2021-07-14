#ifndef XLIB_IO_SUPPORT_IOERROR_HPP_
#define XLIB_IO_SUPPORT_IOERROR_HPP_

#include <string>
#include <system_error>

#include "xlib/io/support/io_errc.hpp"

namespace xlib::io {

namespace detail {

class __io_error_category_impl final : public std::error_category {
 public:
  constexpr __io_error_category_impl() noexcept = default;

  [[nodiscard]] const char* name() const noexcept override { return "io"; }

  [[nodiscard]] std::string message(int ev) const override {
    auto value = static_cast<io_errc>(ev);
    switch (value) {
      case io_errc::bad_file_descriptor:
        return "bad file descriptor";
      case io_errc::invalid_argument:
        return "invalid argument";
      case io_errc::value_too_large:
        return "value_too_large";
      case io_errc::reached_end_of_file:
        return "reached end of file";
      case io_errc::interrupted:
        return "interrupted";
      case io_errc::physical_error:
        return "psysical error";
      case io_errc::file_too_large:
        return "file too large";
      default:
        return "unknown error";
    }
  }
};

}  // namespace detail

inline const std::error_category& category() noexcept {
  static detail::__io_error_category_impl instance;
  return instance;
}

error_code make_error_code(io_errc e) noexcept {
  return {static_cast<int>(e), io::category()};
}

std::error_condition make_error_condition(io_errc e) noexcept {
  return {static_cast<int>(e), io::category()};
}

class io_error : public std::system_error {
 public:
  io_error(const std::string& message, error_code ec) : system_error(ec, message){};
  io_error(const char* message, error_code ec) : system_error(ec, message){};
};

}  // namespace xlib::io

#endif  // XLIB_IO_SUPPORT_IOERROR_HPP_
