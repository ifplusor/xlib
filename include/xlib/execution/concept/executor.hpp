#ifndef XLIB_EXECUTION_CONCEPT_EXECUTOR_HPP_
#define XLIB_EXECUTION_CONCEPT_EXECUTOR_HPP_

#include "xlib/concepts/concept.hpp"

#include <utility>

#include "xlib/concepts/constructible_from.hpp"
#include "xlib/concepts/copy_constructible.hpp"
#include "xlib/concepts/equality_comparable.hpp"
#include "xlib/concepts/invocable.hpp"
#include "xlib/concepts/move_constructible.hpp"
#include "xlib/execution/executor/execute.hpp"
#include "xlib/execution/support/invocable_archetype.hpp"
#include "xlib/types/type_traits/is_copy_constructible.hpp"
#include "xlib/types/type_traits/is_invocable.hpp"
#include "xlib/types/type_traits/remove_cvref.hpp"
#include "xlib/types/type_traits/void.hpp"

namespace xlib::execution {

namespace detail {

#if defined(_XLIB_CONCEPTS)

template <typename E, typename F>
xlib_concept __can_execute = requires(const E& e, F&& f) {
  ::xlib::execution::execute(e, std::forward<F>(f));
};

#else  // defined(_XLIB_CONCEPTS)

template <typename E, typename F>
xlib_concept __can_execute = is_invocable_v<decltype(::xlib::execution::execute), E, F>;

#endif  // defined(_XLIB_CONCEPTS)

template <typename E, typename F>
xlib_concept __executor_of_impl =
    invocable<remove_cvref_t<F>&> && constructible_from<remove_cvref_t<F>, F> && move_constructible<remove_cvref_t<F>> &&
        copy_constructible<E> && is_nothrow_copy_constructible_v<E> && equality_comparable<E> && __can_execute<E, F>;

}  // namespace detail

/**
 * @brief The concept of executor
 *
 * @tparam E type of executor
 *
 * @note The operationl semantics of `execution::execute` should be specified with the `execution::execute` CPO rather
 *       than the executor concept.
 */
template <class E>
xlib_concept executor = detail::__executor_of_impl<E, invocable_archetype>;

template <class E, class F>
xlib_concept executor_of = executor<E>&& detail::__executor_of_impl<E, F>;

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_CONCEPT_EXECUTOR_HPP_
