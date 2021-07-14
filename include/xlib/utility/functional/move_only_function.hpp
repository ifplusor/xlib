#ifndef XLIB_UTILITY_FUNCTIONAL_MOVEONLYFUNCTION_HPP_
#define XLIB_UTILITY_FUNCTIONAL_MOVEONLYFUNCTION_HPP_

#include <cstddef>  // std::nullptr_t

#include <functional>  // std::reference_wrapper
#include <optional>
#include <tuple>
#include <type_traits>  // std::conjunction, std::is_invocable_r
#include <utility>      // std::forward, std::move

#include "xlib/types/type_traits/decay.hpp"
#include "xlib/types/type_traits/enable_if.hpp"
#include "xlib/types/type_traits/integral_constant.hpp"
#include "xlib/types/type_traits/is_constructible.hpp"
#include "xlib/types/type_traits/is_move_constructible.hpp"
#include "xlib/types/type_traits/is_same.hpp"
#include "xlib/types/type_traits/negation.hpp"
#include "xlib/types/type_traits/remove_cvref.hpp"
#include "xlib/utility/functional/function_storage.hpp"

namespace xlib {

namespace detail {

template <typename T>
struct is_in_place_type : false_type {};

template <typename T>
struct is_in_place_type<std::in_place_type_t<T>> : true_type {};

template <typename T>
inline constexpr auto is_in_place_type_v = is_in_place_type<T>::value;

template <typename R, bool is_noexcept, typename... ArgTypes>
class __move_only_function_impl {
  template <typename T>
  using function_type = typename __function_storage_traits<T>::function_type;
  using storage_type = detail::__function_storage;

  using destory_function = void (*)(storage_type&);
  using move_function = void (*)(storage_type&, storage_type&);
  using invoke_function = R (*)(storage_type&, ArgTypes...);
  using storage_function = std::optional<std::tuple<destory_function, move_function, invoke_function>>;

 public:
  using result_type = R;

  __move_only_function_impl() noexcept = default;

  __move_only_function_impl(std::nullptr_t) noexcept {}
  __move_only_function_impl& operator=(std::nullptr_t) noexcept {
    destroy();
    return *this;
  }

  // Disable copy
  __move_only_function_impl(const __move_only_function_impl&) = delete;
  __move_only_function_impl& operator=(const __move_only_function_impl&) = delete;

  // Enable move
  __move_only_function_impl(__move_only_function_impl&& other) noexcept : function_(std::move(other.function_)) {
    if (function_) {
      std::get<1> (*function_)(storage_, other.storage_);
      other.function_.reset();
    }
  }
  __move_only_function_impl& operator=(__move_only_function_impl&& other) noexcept {
    __move_only_function_impl{std::move(other)}.swap(*this);
    return *this;
  }

  ~__move_only_function_impl() { destroy(); }

  void swap(__move_only_function_impl& other) noexcept {
    if (function_) {
      if (other.function_) {
        storage_type tmp;
        std::get<1> (*function_)(tmp, storage_);
        std::get<1> (*other.function_)(storage_, other.storage_);
        std::get<1> (*function_)(other.storage_, tmp);
        function_.swap(other.function_);
      } else {
        other.swap(*this);
      }
    } else if (other.function_) {
      std::get<1> (*other.function_)(storage_, other.storage_);
      function_.swap(other.function_);
    }
  }

  explicit operator bool() const noexcept { return function_.has_value(); }

 protected:
  template <typename F, typename... Args>
  void create(Args&&... args) {
    using storage_function = function_type<F>;
    storage_function::create(storage_, std::forward<Args>(args)...);
    function_ = std::make_tuple<destory_function, move_function, invoke_function>(
        &storage_function::destroy, &storage_function::move, &storage_function::invoke);
  }

  void destroy() noexcept {
    if (function_) {
      std::get<0> (*function_)(storage_);
      function_.reset();
    }
  }

  R invoke(ArgTypes... args) noexcept(is_noexcept) {
    return std::get<2>(*function_)(storage_, std::forward<ArgTypes>(args)...);
  }

  friend bool operator==(const __move_only_function_impl& f, std::nullptr_t) noexcept { return !f; }
  friend bool operator==(std::nullptr_t, const __move_only_function_impl& f) noexcept { return !f; }
  friend bool operator!=(const __move_only_function_impl& f, std::nullptr_t) noexcept { return static_cast<bool>(f); }
  friend bool operator!=(std::nullptr_t, const __move_only_function_impl& f) noexcept { return static_cast<bool>(f); }

  friend void swap(__move_only_function_impl& lhs, __move_only_function_impl& rhs) noexcept { lhs.swap(rhs); }

 private:
  storage_type storage_;
  storage_function function_;
};

template <typename AI, typename F, bool noex, typename R, typename FCall, typename... ArgTypes>
using __function_can_convert =
    std::conjunction<negation<is_same<remove_cvref_t<F>, AI>>,
                     negation<detail::is_in_place_type<remove_cvref_t<F>>>,
                     std::is_invocable_r<R, FCall, ArgTypes...>,
                     bool_constant<(!noex || std::is_nothrow_invocable_r_v<R, FCall, ArgTypes...>)>,
                     is_constructible<decay_t<F>, F>>;

}  // namespace detail

template <typename Function>
class move_only_function;

#define __XLIB_MOVEONLYFUNCTION_TEMPLATE(cv, ref, noex, inv_quals)                                                   \
  template <typename R, typename... ArgTypes>                                                                        \
  class move_only_function<R(ArgTypes...) cv ref noexcept(noex)>                                                     \
      : public detail::__move_only_function_impl<R, noex, ArgTypes...> {                                             \
    using base_type = detail::__move_only_function_impl<R, noex, ArgTypes...>;                                       \
                                                                                                                     \
   public:                                                                                                           \
    using base_type::base_type;                                                                                      \
                                                                                                                     \
    template <typename F,                                                                                            \
              typename = enable_if_t<                                                                                \
                  detail::__function_can_convert<move_only_function, F, noex, R, F inv_quals, ArgTypes...>::value>>  \
    move_only_function(F&& f) {                                                                                      \
      base_type::template create<decay_t<F>>(std::forward<F>(f));                                                    \
    }                                                                                                                \
                                                                                                                     \
    template <typename T,                                                                                            \
              typename... Args,                                                                                      \
              typename VT = decay_t<T>,                                                                              \
              typename = enable_if_t<is_move_constructible_v<VT> && is_constructible_v<VT, Args...> &&               \
                                     std::is_invocable_r_v<R, VT inv_quals, ArgTypes...> &&                          \
                                     (!(noex) || std::is_nothrow_invocable_r_v<R, VT inv_quals, ArgTypes...>)>>      \
    explicit move_only_function(std::in_place_type_t<T>, Args&&... args) {                                           \
      base_type::template create<VT>(std::forward<Args>(args)...);                                                   \
    }                                                                                                                \
                                                                                                                     \
    template <typename T,                                                                                            \
              typename U,                                                                                            \
              typename... Args,                                                                                      \
              typename VT = decay_t<T>,                                                                              \
              typename = enable_if_t<is_move_constructible_v<VT> &&                                                  \
                                     is_constructible_v<VT, std::initializer_list<U>&, Args...> &&                   \
                                     std::is_invocable_r_v<R, VT inv_quals, ArgTypes...> &&                          \
                                     (!(noex) || std::is_nothrow_invocable_r_v<R, VT inv_quals, ArgTypes...>)>>      \
    explicit move_only_function(std::in_place_type_t<T>, std::initializer_list<U> il, Args&&... args) {              \
      base_type::template create<VT>(il, std::forward<Args>(args)...);                                               \
    }                                                                                                                \
                                                                                                                     \
    template <typename F, typename FDec = decay_t<F>>                                                                \
    auto operator=(F&& f)                                                                                            \
        -> enable_if_t<!is_same_v<FDec, move_only_function> && is_move_constructible_v<FDec>, move_only_function&> { \
      move_only_function{std::forward<F>(f)}.swap(*this);                                                            \
      return *this;                                                                                                  \
    }                                                                                                                \
    template <typename F>                                                                                            \
    move_only_function& operator=(std::reference_wrapper<F> f) {                                                     \
      move_only_function{f}.swap(*this);                                                                             \
      return *this;                                                                                                  \
    }                                                                                                                \
                                                                                                                     \
    R operator()(ArgTypes... args) cv ref noexcept(noex) {                                                           \
      return base_type::invoke(std::forward<ArgTypes>(args)...);                                                     \
    }                                                                                                                \
  }

// cv -> {`empty`, const}
// ref -> {`empty`, &, &&}
// noex -> {true, false}
// inv_quals -> (is_empty(ref) ? & : ref)
__XLIB_MOVEONLYFUNCTION_TEMPLATE(, , false, &);               // 000
__XLIB_MOVEONLYFUNCTION_TEMPLATE(, , true, &);                // 001
__XLIB_MOVEONLYFUNCTION_TEMPLATE(, &, false, &);              // 010
__XLIB_MOVEONLYFUNCTION_TEMPLATE(, &, true, &);               // 011
__XLIB_MOVEONLYFUNCTION_TEMPLATE(, &&, false, &&);            // 020
__XLIB_MOVEONLYFUNCTION_TEMPLATE(, &&, true, &&);             // 021
__XLIB_MOVEONLYFUNCTION_TEMPLATE(const, , false, const&);     // 100
__XLIB_MOVEONLYFUNCTION_TEMPLATE(const, , true, const&);      // 101
__XLIB_MOVEONLYFUNCTION_TEMPLATE(const, &, false, const&);    // 110
__XLIB_MOVEONLYFUNCTION_TEMPLATE(const, &, true, const&);     // 111
__XLIB_MOVEONLYFUNCTION_TEMPLATE(const, &&, false, const&&);  // 120
__XLIB_MOVEONLYFUNCTION_TEMPLATE(const, &&, true, const&&);   // 121

#undef __XLIB_MOVEONLYFUNCTION_TEMPLATE

}  // namespace xlib

#endif  // XLIB_UTILITY_FUNCTIONAL_MOVEONLYFUNCTION_HPP_
