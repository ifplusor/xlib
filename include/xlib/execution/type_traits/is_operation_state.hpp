#ifndef XLIB_EXECUTION_TYPETRAITS_ISOPERATIONSTATE_HPP_
#define XLIB_EXECUTION_TYPETRAITS_ISOPERATIONSTATE_HPP_

#include "xlib/__config.hpp"
#include "xlib/execution/concept/operation_state.hpp"
#include "xlib/types/type_traits/integral_constant.hpp"

namespace xlib::execution::detail {

template <typename OperationState>
struct is_operation_state : std::conditional_t<operation_state<OperationState>, true_type, false_type> {};

#if defined(_XLIB_INLINE_VARIABLES)

template <typename OperationState>
_XLIB_INLINE_VAR constexpr bool is_operation_state_v = is_operation_state<OperationState>::value;

#endif  // defined(_XLIB_INLINE_VARIABLES)

}  // namespace xlib::execution::detail

#endif  // XLIB_EXECUTION_TYPETRAITS_ISOPERATIONSTATE_HPP_
