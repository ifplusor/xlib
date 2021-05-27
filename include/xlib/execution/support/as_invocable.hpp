#ifndef XLIB_EXECUTION_SUPPORT_ASINVOCABLE_HPP_
#define XLIB_EXECUTION_SUPPORT_ASINVOCABLE_HPP_

#include <exception>  // std::current_excption
#include <memory>     // std::addressof
#include <utility>    // std::move

#include "xlib/execution/executor/execute.hpp"
#include "xlib/execution/receiver/set_done.hpp"
#include "xlib/execution/receiver/set_error.hpp"
#include "xlib/execution/receiver/set_value.hpp"
#include "xlib/types/type_traits/remove_cvref.hpp"

namespace xlib::execution {

template <typename Receiver, typename Executor = void>
struct as_invocable {
  Receiver* receiver;

  explicit as_invocable(Receiver& r) noexcept : receiver(std::addressof(r)) {}

  ~as_invocable() {
    if (receiver != nullptr) {
      ::xlib::execution::set_done(std::move(*receiver));
    }
  }

  as_invocable(const as_invocable&) = delete;
  as_invocable& operator=(const as_invocable&) = delete;

  as_invocable(as_invocable&& other) noexcept : receiver(std::exchange(other.receiver, nullptr)) {}
  as_invocable& operator=(as_invocable&&) noexcept = delete;

  void operator()() & noexcept {
    try {
      ::xlib::execution::set_value(std::move(*receiver));
    } catch (...) {
      ::xlib::execution::set_error(std::move(*receiver), std::current_exception());
    }
    receiver = nullptr;
  }
};

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_SUPPORT_ASINVOCABLE_HPP_
