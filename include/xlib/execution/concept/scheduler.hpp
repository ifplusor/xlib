#ifndef XLIB_EXECUTION_CONCEPT_SCHEDULER_HPP_
#define XLIB_EXECUTION_CONCEPT_SCHEDULER_HPP_

#include "xlib/__config.hpp"

#if defined(_XLIB_VARIABLE_TEMPLATES)

#include <exception>
#include <utility>

#include "xlib/concepts/concept.hpp"
#include "xlib/concepts/copy_constructible.hpp"
#include "xlib/concepts/equality_comparable.hpp"
#include "xlib/execution/scheduler/schedule.hpp"
#include "xlib/types/type_traits/remove_cvref.hpp"

namespace xlib {
namespace execution {

namespace detail {

// clang-format off
template <typename S>
xlib_requires(__scheduler_helper,
requires (S&& s) (
  ::xlib::execution::schedule(std::forward<S>(s))
));
// clang-format on

}

template <typename S>
xlib_concept scheduler = copy_constructible<remove_cvref_t<S>> && equality_comparable<remove_cvref_t<S>> &&
    xlib_requires_ref(detail::__scheduler_helper, S);

}  // namespace execution
}  // namespace xlib

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

#endif  // XLIB_EXECUTION_CONCEPT_SCHEDULER_HPP_
