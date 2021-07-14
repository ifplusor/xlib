#ifndef XLIB_EXECUTION_PROPERTY_MAPPING_HPP_
#define XLIB_EXECUTION_PROPERTY_MAPPING_HPP_

#include "xlib/execution/property/behavioral_property.hpp"

namespace xlib::execution {

/**
 * @brief Describes what guarantees executors provide about the mapping of execution agents onto threads of execution.
 */
struct mapping_t : detail::__behavioral_propety_impl<mapping_t, 3> {
  using detail::__behavioral_propety_impl<mapping_t, 3>::__behavioral_propety_impl;

  using thread_t = enumerator<0>;
  using new_thread_t = enumerator<1>;
  using other_t = enumerator<1>;

  static constexpr thread_t thread{};
  static constexpr new_thread_t new_thread{};
  static constexpr other_t other{};
};

constexpr mapping_t mapping{};

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_PROPERTY_MAPPING_HPP_
