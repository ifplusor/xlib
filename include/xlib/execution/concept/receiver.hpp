#ifndef XLIB_EXECUTION_CONCEPT_RECEIVER_HPP_
#define XLIB_EXECUTION_CONCEPT_RECEIVER_HPP_

#include "xlib/concepts/concept.hpp"

#include <exception>  // std::exeception
#include <utility>    // std::move, std::forward

#include "xlib/concepts/constructible_from.hpp"
#include "xlib/concepts/invocable.hpp"
#include "xlib/concepts/move_constructible.hpp"
#include "xlib/execution/receiver/set_done.hpp"
#include "xlib/execution/receiver/set_error.hpp"
#include "xlib/execution/receiver/set_value.hpp"
#include "xlib/types/type_traits/is_invocable.hpp"
#include "xlib/types/type_traits/remove_cvref.hpp"
#include "xlib/types/type_traits/void.hpp"

namespace xlib::execution {

namespace detail {

#if defined(_XLIB_CONCEPTS)

template <typename T, typename E = std::exception_ptr>
xlib_concept __can_set_done_and_error = requires(remove_cvref_t<T>&& t, E&& e) {
  { ::xlib::execution::set_done(std::move(t)) } noexcept;
  { ::xlib::execution::set_error(std::move(t), std::forward<E>(e)) } noexcept;
};

template <typename T, typename... An>
xlib_concept __can_set_value = requires(remove_cvref_t<T>&& t, An&&... an) {
  ::xlib::execution::set_value(std::move(t), std::forward<An>(an)...);
};

#else  // defined(_XLIB_CONCEPTS)

template <typename T, typename E = std::exception_ptr>
xlib_concept __can_set_done_and_error =
    is_nothrow_invocable_v<decltype(::xlib::execution::set_done), remove_cvref_t<T>&&> &&
        is_nothrow_invocable_v<decltype(::xlib::execution::set_error), remove_cvref_t<T>&&, E>;

template <class T, class... An>
xlib_concept __can_set_value = is_invocable_v<decltype(::xlib::execution::set_value), remove_cvref_t<T>&&, An...>;

#endif  // defined(_XLIB_CONCEPTS)

}  // namespace detail

template <class T, class E = std::exception_ptr>
xlib_concept receiver = move_constructible<remove_cvref_t<T>>&& constructible_from<remove_cvref_t<T>, T>&&
    detail::__can_set_done_and_error<T, E>;

template <class T, class... An>
xlib_concept receiver_of = receiver<T>&& detail::__can_set_value<T, An...>;

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_CONCEPT_RECEIVER_HPP_
