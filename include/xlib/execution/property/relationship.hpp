#ifndef XLIB_EXECUTION_PROPERTY_RELATIONSHIP_HPP_
#define XLIB_EXECUTION_PROPERTY_RELATIONSHIP_HPP_

#include "xlib/execution/property/behavioral_property.hpp"

namespace xlib::execution {

/**
 * @brief Allows users of executors to indicate that submitted tasks represent continuations.
 */
struct relationship_t : detail::__behavioral_propety_impl<relationship_t, 2> {
  using detail::__behavioral_propety_impl<relationship_t, 2>::__behavioral_propety_impl;

  using fork_t = enumerator<0>;
  using continuation_t = enumerator<1>;

  static constexpr fork_t fork{};
  static constexpr continuation_t continuation{};
};

constexpr relationship_t relationship{};

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_PROPERTY_RELATIONSHIP_HPP_
