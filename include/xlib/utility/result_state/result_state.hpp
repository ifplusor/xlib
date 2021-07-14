#ifndef XLIB_UTILITY_RESULTSTATE_HPP_
#define XLIB_UTILITY_RESULTSTATE_HPP_

#include <exception>  // std::exception_ptr, std::rethrow_exception
#include <stdexcept>  // std::runtime_error
#include <utility>    // std::move, std::forward

#include "xlib/types/type_traits/decay.hpp"

namespace xlib {

namespace detail {

template <typename R>
class __result_state_impl {
 public:
  // Types:
  using result_type = R;

  __result_state_impl() = default;
  __result_state_impl(result_type result) : has_result_(true), result_(std::move(result)) {}
  __result_state_impl(const std::exception_ptr& exception) : has_result_(true), exception_(exception) {}

  ~__result_state_impl() = default;

  // Disable copy
  __result_state_impl(const __result_state_impl&) = delete;
  __result_state_impl& operator=(const __result_state_impl&) = delete;

  // Enable move
  __result_state_impl(__result_state_impl&&) noexcept = default;
  __result_state_impl& operator=(__result_state_impl&&) noexcept = default;

  const result_type& get() const& {
    if (exception_ != nullptr) {
      std::rethrow_exception(exception_);
    }
    return result_;
  }

  result_type& get() & {
    if (exception_ != nullptr) {
      std::rethrow_exception(exception_);
    }
    return result_;
  }

  result_type&& get() && {
    if (exception_ != nullptr) {
      std::rethrow_exception(exception_);
    }
    return result_;
  }

  template <typename T = result_type>
  void set_result(T&& result) {
    check_state();
    result_ = std::forward<T>(result);
    has_result_ = true;
  }

  void set_exception(const std::exception_ptr& exception) {
    check_state();
    exception_ = exception;
    has_result_ = true;
  }

  [[nodiscard]] bool has_result() const { return has_result_; }

 private:
  void check_state() {
    if (has_result_) {
      throw std::runtime_error("state has set value already");
    }
  }

 private:
  bool has_result_{false};
  result_type result_;
  std::exception_ptr exception_;
};

template <typename R>
class __result_state_impl<R&> {
  // TODO
};

template <>
class __result_state_impl<void> {
  // TODO
};

}  // namespace detail

template <typename T>
using result_state = detail::__result_state_impl<T>;

}  // namespace xlib

#endif  // XLIB_UTILITY_RESULTSTATE_HPP_
