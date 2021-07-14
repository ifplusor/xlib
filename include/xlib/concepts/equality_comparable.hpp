#ifndef XLIB_CONCEPTS_EQUALITYCOMPARABLE_HPP_
#define XLIB_CONCEPTS_EQUALITYCOMPARABLE_HPP_

#include <utility>
#include "xlib/concepts/concept.hpp"

#if defined(_XLIB_LIB_CONCEPTS)

#include <concepts>

namespace xlib {

using std::equality_comparable;
using std::equality_comparable_with;

}  // namespace xlib

#else  // defined(_XLIB_LIB_CONCEPTS)

#include "xlib/concepts/boolean_testable.hpp"
#include "xlib/concepts/common_reference_with.hpp"
#include "xlib/types/type_traits/common_reference.hpp"
#include "xlib/types/type_traits/remove_reference.hpp"

namespace xlib {

namespace detail {

#if defined(_XLIB_CONCEPTS)

template <typename T, typename U>
xlib_concept __weakly_equality_comparable_with = requires(const remove_reference_t<T>& t, const remove_reference_t<U>& u) {
  { t == u } -> boolean_testable;
  { t != u } -> boolean_testable;
  { u == t } -> boolean_testable;
  { u != t } -> boolean_testable;
};

#else  // defined(_XLIB_CONCEPTS)

template <typename T, typename U>
xlib_concept __weakly_equality_comparable_with =
    boolean_testable<decltype(std::declval<remove_reference_t<T>>() == std::declval<remove_reference_t<U>>())>
    && boolean_testable<decltype(std::declval<remove_reference_t<T>>() != std::declval<remove_reference_t<U>>())>
    && boolean_testable<decltype(std::declval<remove_reference_t<U>>() == std::declval<remove_reference_t<T>>())>
    && boolean_testable<decltype(std::declval<remove_reference_t<U>>() != std::declval<remove_reference_t<T>>())>;

#endif  // defined(_XLIB_CONCEPTS)

}  // namespace detail

template <typename T>
xlib_concept equality_comparable = detail::__weakly_equality_comparable_with<T, T>;

template <typename T, typename U>
xlib_concept equality_comparable_with = equality_comparable<T>
    && equality_comparable<U>
    && common_reference_with<const remove_reference_t<T>&, const remove_reference_t<U>&>
    && equality_comparable<common_reference_t<const remove_reference_t<T>&, const remove_reference_t<U>&>>
    && detail::__weakly_equality_comparable_with<T, U>;

}  // namespace xlib

#endif  // defined(_XLIB_LIB_CONCEPTS)

#endif  // XLIB_CONCEPTS_EQUALITYCOMPARABLE_HPP_
