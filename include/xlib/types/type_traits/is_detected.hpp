#ifndef XLIB_TYPES_TYPETRAITS_ISBASEOF_HPP_
#define XLIB_TYPES_TYPETRAITS_ISBASEOF_HPP_

#include "xlib/__config.hpp"

// clang-format off
#if _XLIB_HAS_INCLUDE(<experimental/type_traits>)
// clang-format on

#include <experimental/type_traits>

#if !defined(__cpp_lib_experimental_detect) || __cpp_lib_experimental_detect >= 201505L
#define _XLIB_LIB_DETECT

namespace xlib {

using std::experimental::is_detected;

using std::experimental::detected_t;

using std::experimental::detected_or;
using std::experimental::detected_or_t;

using std::experimental::is_detected_exact;

using std::experimental::is_detected_convertible;

#if defined(_XLIB_VARIABLE_TEMPLATES)

using std::experimental::is_detected_v;

using std::experimental::is_detected_exact_v;

using std::experimental::is_detected_convertible_v;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#endif  // !defined(__cpp_lib_experimental_detect) || __cpp_lib_experimental_detect >= 201505L
#endif  // _XLIB_HAS_INCLUDE(<experimental/type_traits>)

#if !defined(_XLIB_LIB_DETECT)
#define _XLIB_LIB_DETECT

#include "xlib/types/type_traits/integral_constant.hpp"
#include "xlib/types/type_traits/is_convertible.hpp"
#include "xlib/types/type_traits/is_same.hpp"
#include "xlib/types/type_traits/nonesuch.hpp"
#include "xlib/types/type_traits/void.hpp"

namespace xlib {

namespace detail {

template <typename Default, typename AlwaysVoid, template <typename...> typename Op, typename... Args>
struct __detector {
  using value_t = false_type;
  using type = Default;
};

template <typename Default, template <typename...> typename Op, typename... Args>
struct __detector<Default, void_t<Op<Args...>>, Op, Args...> {
  using value_t = true_type;
  using type = Op<Args...>;
};

}  // namespace detail

template <template <typename...> typename Op, typename... Args>
using is_detected = typename detail::__detector<nonesuch, void, Op, Args...>::value_t;

template <template <typename...> typename Op, typename... Args>
using detected_t = typename detail::__detector<nonesuch, void, Op, Args...>::type;

template <typename Default, template <typename...> typename Op, typename... Args>
using detected_or = detail::__detector<Default, void, Op, Args...>;

template <typename Default, template <typename...> typename Op, typename... Args>
using detected_or_t = typename detected_or<Default, Op, Args...>::type;

template <typename Expected, template <typename...> typename Op, typename... Args>
using is_detected_exact = is_same<Expected, detected_t<Op, Args...>>;

template <typename To, template <typename...> typename Op, typename... Args>
using is_detected_convertible = is_convertible<detected_t<Op, Args...>, To>;

#if defined(_XLIB_VARIABLE_TEMPLATES)

template <template <typename...> typename Op, typename... Args>
_XLIB_INLINE_VAR constexpr bool is_detected_v = is_detected<Op, Args...>::value;

template <typename Expected, template <typename...> typename Op, typename... Args>
_XLIB_INLINE_VAR constexpr bool is_detected_exact_v = is_detected_exact<Expected, Op, Args...>::value;

template <typename To, template <typename...> typename Op, typename... Args>
_XLIB_INLINE_VAR constexpr bool is_detected_convertible_v = is_detected_convertible<To, Op, Args...>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#endif  // !defined(_XLIB_LIB_DETECT)

#endif  // XLIB_TYPES_TYPETRAITS_ISBASEOF_HPP_
