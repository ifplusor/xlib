#ifndef XLIB_CONCEPTS_BOOLEANTESTABLE_HPP_
#define XLIB_CONCEPTS_BOOLEANTESTABLE_HPP_

#include "xlib/__config.hpp"

#if defined(_XLIB_VARIABLE_TEMPLATES)

#include <utility>

#include "xlib/concepts/concept.hpp"
#include "xlib/concepts/convertible_to.hpp"

namespace xlib {

namespace detail {

template <typename B>
xlib_concept __boolean_testable_impl = convertible_to<B, bool>;

template <typename B>
auto __boolean_testable_helper(B&& b) {
  return !std::forward<B>(b);
}

}  // namespace detail

template <typename B>
xlib_concept boolean_testable = detail::__boolean_testable_impl<B> &&
    detail::__boolean_testable_impl<decltype(detail::__boolean_testable_helper(std::declval<B>()))>;

}  // namespace xlib

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

#endif  // XLIB_CONCEPTS_BOOLEANTESTABLE_HPP_
