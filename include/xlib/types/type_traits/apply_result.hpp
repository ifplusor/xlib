#ifndef XLIB_TYPES_TYPETRAITS_APPLYRESULT_HPP_
#define XLIB_TYPES_TYPETRAITS_APPLYRESULT_HPP_

#include "xlib/meta/type_list.hpp"
#include "xlib/types/type_traits/enable_if.hpp"
#include "xlib/types/type_traits/invoke_result.hpp"
#include "xlib/types/type_traits/is_void.hpp"

namespace xlib {

namespace detail {

template <typename Result, typename = void>
struct __apply_result_overload {
  using type = type_list<Result>;
};

template <typename Result>
struct __apply_result_overload<Result, enable_if_t<is_void_v<Result>>> {
  using type = type_list<>;
};

template <typename Function, typename TypeList>
struct __apply_result_impl {
  template <typename... Arguments>
  using invoke_result_type = invoke_result_t<Function, Arguments...>;

  using type = typename __apply_result_overload<typename TypeList::template apply<invoke_result_type>>::type;
};

}  // namespace detail

template <typename Function, typename TypeList>
using apply_result = detail::__apply_result_impl<Function, TypeList>;

template <typename Function, typename TypeList>
using apply_result_t = typename apply_result<Function, TypeList>::type;

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_APPLYRESULT_HPP_
