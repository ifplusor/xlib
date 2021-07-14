#ifndef XLIB_PROPERTY_ISAPPLICABLEPROPERTY_HPP_
#define XLIB_PROPERTY_ISAPPLICABLEPROPERTY_HPP_

#include "xlib/__config.hpp"
#include "xlib/types/type_traits/enable_if.hpp"
#include "xlib/types/type_traits/integral_constant.hpp"
#include "xlib/types/type_traits/void.hpp"

namespace xlib {

namespace detail {

template <typename Entity, typename Property, typename = void_t<>>
struct __is_applicable_property_impl : false_type {};

template <typename Entity, typename Property>
struct __is_applicable_property_impl<Entity, Property, enable_if_t<Property::template is_applicable_property_v<Entity>>>
    : true_type {};

}  // namespace detail

template <typename Entity, typename Property>
using is_applicable_property = detail::__is_applicable_property_impl<Entity, Property>;

#if defined(_XLIB_VARIABLE_TEMPLATES)

template <typename Entity, typename Property>
_XLIB_INLINE_VAR constexpr bool is_applicable_property_v = is_applicable_property<Entity, Property>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#endif  // XLIB_PROPERTY_ISAPPLICABLEPROPERTY_HPP_
