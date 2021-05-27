#ifndef XLIB_EXECUTION_TYPETRAITS_SENDERVALUE_HPP_
#define XLIB_EXECUTION_TYPETRAITS_SENDERVALUE_HPP_

#include "xlib/meta_container/type_list.hpp"
#include "xlib/execution/type_traits/sender_traits.hpp"
#include "xlib/types/type_traits/conditional.hpp"
#include "xlib/types/type_traits/decay.hpp"
#include "xlib/types/type_traits/enable_if.hpp"
#include "xlib/types/type_traits/wrap_reference.hpp"

namespace xlib::execution::detail {

template <typename Sender>
struct sender_value_traits {
  using value_types = typename sender_traits<Sender>::template value_types<type_list, type_list>::template at<0>;
  using error_types = typename sender_traits<Sender>::template error_types<type_list>::template at<0>;
};

template <typename Sender>
using sender_single_value_type_t =
    conditional_t<sender_value_traits<Sender>::value_types::size() == 0,
                  void,
                  enable_if_t<sender_value_traits<Sender>::value_types::size() == 1,
                              typename sender_value_traits<Sender>::value_types::template at<0>>>;

template <typename Sender>
using sender_single_value_result_t = wrap_reference_t<decay_rvalue_t<sender_single_value_type_t<Sender>>>;

}  // namespace xlib::execution::detail

#endif  // XLIB_EXECUTION_TYPETRAITS_SENDERVALUE_HPP_
