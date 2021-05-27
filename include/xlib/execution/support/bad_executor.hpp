#ifndef XLIB_EXECUTION_SUPPORT_BADEXECUTOR_HPP_
#define XLIB_EXECUTION_SUPPORT_BADEXECUTOR_HPP_

#include <exception>

namespace xlib::execution {

class bad_executor : public std::exception {
 public:
  bad_executor() noexcept = default;

  [[nodiscard]] const char* what() const noexcept override { return "bad executor"; }
};

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_SUPPORT_BADEXECUTOR_HPP_
