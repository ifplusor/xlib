#ifndef XLIB_EXECUTION_PROPERTY_BEHAVIORALPROPERTY_HPP_
#define XLIB_EXECUTION_PROPERTY_BEHAVIORALPROPERTY_HPP_

#include "xlib/property/enumeration.hpp"
#include "xlib/execution/concept/executor.hpp"

namespace xlib::execution::detail {

template <typename Property, int N>
struct __behavioral_propety_impl : enumeration<Property, N> {
  using enumeration<Property, N>::enumeration;

  template <typename T>
  static constexpr bool is_applicable_property_v = executor<T>;
};

}  // namespace xlib::execution::detail

#endif  // XLIB_EXECUTION_PROPERTY_BEHAVIORALPROPERTY_HPP_
