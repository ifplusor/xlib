#ifndef XLIB_TYPES_TYPETRAITS_ISINVOCABLE_HPP_
#define XLIB_TYPES_TYPETRAITS_ISINVOCABLE_HPP_

#include "xlib/__config.hpp"

#if _XLIB_STD_VER >= 2017L

#include <type_traits>

namespace xlib {

using std::is_invocable;
using std::is_invocable_v;

using std::is_nothrow_invocable;
using std::is_nothrow_invocable_v;

}  // namespace xlib

#else  // _XLIB_STD_VER >= 2017L

#include <utility>  // std::declval

#include "xlib/types/type_traits/conditional.hpp"
#include "xlib/types/type_traits/enable_if.hpp"
#include "xlib/types/type_traits/integral_constant.hpp"
#include "xlib/utility/functional/invoke_op.hpp"

namespace xlib {

template <class Fn, class... ArgTypes>
struct is_invocable : public conditional_t<(detail::INVOKE(std::declval<Fn>(), std::declval<ArgTypes>()...), true),
                                           true_type,
                                           false_type> {};

template <class Fn, class... ArgTypes>
struct is_nothrow_invocable
    : public conditional_t<noexcept(detail::INVOKE(std::declval<Fn>(), std::declval<ArgTypes>()...)),
                           true_type,
                           false_type> {};

#if defined(_XLIB_VARIABLE_TEMPLATES)

template <class Fn, class... ArgTypes>
_XLIB_INLINE_VAR constexpr bool is_invocable_v = is_invocable<Fn, ArgTypes...>::value;

template <class Fn, class... ArgTypes>
_XLIB_INLINE_VAR constexpr bool is_nothrow_invocable_v = is_nothrow_invocable<Fn, ArgTypes...>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#endif  // _XLIB_STD_VER >= 2017L

#if _XLIB_STD_VER >= 2017L || defined(_XLIB_VARIABLE_TEMPLATES)
#define xlib_is_invocable_v(...) xlib::is_invocable_v<__VA_ARGS__>
#define xlib_is_nothrow_invocable_v(...) xlib::is_nothrow_invocable_v<__VA_ARGS__>
#else  // _XLIB_STD_VER >= 2017L || defined(_XLIB_VARIABLE_TEMPLATES)
#define xlib_is_invocable_v(...) xlib::is_invocable<__VA_ARGS__>::value
#define xlib_is_nothrow_invocable_v(...) xlib::is_nothrow_invocable<__VA_ARGS__>::value
#endif  // _XLIB_STD_VER >= 2017L || defined(_XLIB_VARIABLE_TEMPLATES)

#endif  // XLIB_TYPES_TYPETRAITS_ISINVOCABLE_HPP_
