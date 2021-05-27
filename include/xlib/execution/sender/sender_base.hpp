#ifndef XLIB_EXECUTION_SENDER_SENDERBASE_HPP_
#define XLIB_EXECUTION_SENDER_SENDERBASE_HPP_

namespace xlib::execution {

namespace detail {

struct __sender_base_impl {};

}  // namespace detail

using sender_base = detail::__sender_base_impl;

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_SENDER_SENDERBASE_HPP_
