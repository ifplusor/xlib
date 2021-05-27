#ifndef XLIB_EXECUTION_CONCEPT_SENEDERTO_HPP_
#define XLIB_EXECUTION_CONCEPT_SENEDERTO_HPP_

#include "xlib/__config.hpp"
#include "xlib/concepts/concept.hpp"

#if defined(_XLIB_VARIABLE_TEMPLATES)

#include <utility>  // std::forward

#include "xlib/execution/concept/receiver.hpp"
#include "xlib/execution/concept/sender.hpp"
#include "xlib/execution/sender/connect.hpp"

namespace xlib {
namespace execution {

namespace detail {

// clang-format off
template <typename S, typename R>
xlib_requires(__sender_to_helper,
requires (S&& s, R&& r) (
  ::xlib::execution::connect(std::forward<S>(s), std::forward<R>(r))
));
// clang-format on
//
}

template <typename S, typename R>
xlib_concept sender_to = sender<S> && receiver<R> && xlib_requires_ref(detail::__sender_to_helper);

}  // namespace execution
}  // namespace xlib

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

#endif  // XLIB_EXECUTION_CONCEPT_SENEDERTO_HPP_
