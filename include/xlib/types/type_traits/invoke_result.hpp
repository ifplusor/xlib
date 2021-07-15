#ifndef XLIB_TYPES_TYPETRAITS_INVOKERESULT_HPP_
#define XLIB_TYPES_TYPETRAITS_INVOKERESULT_HPP_

#include "xlib/__config.hpp"

#if _XLIB_STD_VER >= 2017L

#include <type_traits>

namespace xlib {

using std::invoke_result;
using std::invoke_result_t;

}  // namespace xlib

#else  // _XLIB_STD_VER >= 2017L

#include <utility>  // std::declval

#include "xlib/utility/functional/invoke_op.hpp"

namespace xlib {

namespace detail {

template <typename AlwaysVoid, typename, typename...>
struct __invoke_result_impl {};

template <typename F, typename... Args>
struct __invoke_result_impl<decltype(void(INVOKE(std::declval<F>(), std::declval<Args>()...))), F, Args...> {
  using type = decltype(INVOKE(std::declval<F>(), std::declval<Args>()...));
};

}  // namespace detail

template <typename F, typename... ArgTypes>
struct invoke_result : detail::__invoke_result_impl<void, F, ArgTypes...> {};

template <typename F, typename... ArgTypes>
using invoke_result_t = typename invoke_result<F, ArgTypes...>::type;

}  // namespace xlib

#endif  // _XLIB_STD_VER >= 2017L

#endif  // XLIB_TYPES_TYPETRAITS_INVOKERESULT_HPP_
