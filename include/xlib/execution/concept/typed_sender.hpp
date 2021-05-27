#ifndef XLIB_EXECUTION_CONCEPT_TYPEDSENEDER_HPP_
#define XLIB_EXECUTION_CONCEPT_TYPEDSENEDER_HPP_

#include "xlib/__config.hpp"

#if defined(_XLIB_CONCEPTS)

#include "xlib/execution/concept/has_sender_types.hpp"
#include "xlib/execution/concept/sender.hpp"
#include "xlib/execution/type_traits/sender_traits.hpp"
#include "xlib/types/type_traits/remove_cvref.hpp"

namespace xlib::execution {

template <typename S>
concept typed_sender = sender<S> && detail::__has_sender_types<sender_traits<remove_cvref_t<S>>>;

}  // namespace xlib::execution

#endif  // defined(_XLIB_CONCEPTS)

#endif  // XLIB_EXECUTION_CONCEPT_TYPEDSENEDER_HPP_
