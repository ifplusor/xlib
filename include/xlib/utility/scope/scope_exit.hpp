#ifndef XLIB_UTILITY_SCOPE_SCOPEEXIT_HPP_
#define XLIB_UTILITY_SCOPE_SCOPEEXIT_HPP_

#include <utility>  // std::move

#include "xlib/types/type_traits/decay.hpp"
#include "xlib/types/type_traits/is_constructible.hpp"
#include "xlib/types/type_traits/is_copy_constructible.hpp"
#include "xlib/types/type_traits/is_move_constructible.hpp"

namespace xlib {

template <typename EF>
class scope_exit {
 public:
  template <typename Fn>
  explicit scope_exit(Fn&& function) noexcept(is_nothrow_constructible<EF, Fn>::value ||
                                              is_nothrow_constructible<EF, Fn&>::value)
      : exit_function_(std::forward<Fn>(function)) {}

  scope_exit(scope_exit&& other) noexcept(is_nothrow_move_constructible<EF>::value ||
                                          is_nothrow_copy_constructible<EF>::value)
      : exit_function_(std::move(other.exit_function_)), engaged_(other.engaged_) {
    other.release();
  }

  scope_exit(const scope_exit&) = delete;

  scope_exit& operator=(scope_exit&&) = delete;
  scope_exit& operator=(const scope_exit&) = delete;

  ~scope_exit() noexcept {
    if (engaged_) {
      try {
        exit_function_();
      } catch (...) {
      }
    }
  }

  void release() noexcept { engaged_ = false; }

 private:
  EF exit_function_;
  bool engaged_{true};
};

template <typename EF>
scope_exit<decay_t<EF>> make_scope_exit(EF&& fn) {
  return scope_exit<decay_t<EF>>(std::forward<EF>(fn));
}

}  // namespace xlib

#endif  // XLIB_UTILITY_SCOPE_SCOPEEXIT_HPP_
