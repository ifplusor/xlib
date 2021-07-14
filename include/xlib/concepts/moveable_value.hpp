#ifndef XLIB_CONCEPTS_MOVEABLEVALUE_HPP_
#define XLIB_CONCEPTS_MOVEABLEVALUE_HPP_

#include "xlib/concepts/concept.hpp"

#include "xlib/concepts/constructible_from.hpp"
#include "xlib/concepts/move_constructible.hpp"
#include "xlib/types/type_traits/remove_cvref.hpp"

namespace xlib {

template <typename T>
xlib_concept moveable_value = move_constructible<remove_cvref_t<T>>&& constructible_from<remove_cvref_t<T>, T>;

}  // namespace xlib

#endif  // XLIB_CONCEPTS_MOVEABLEVALUE_HPP_
