#ifndef XLIB_EXECUTION_PROPERTY_BULKGUARANTEE_HPP_
#define XLIB_EXECUTION_PROPERTY_BULKGUARANTEE_HPP_

#include "xlib/execution/property/behavioral_property.hpp"

namespace xlib::execution {

/**
 * @brief Communicate the forward progress and ordering guarantees of execution agents associated with the bulk
 *        execution.
 */
struct bulk_guarantee_t : detail::__behavioral_propety_impl<bulk_guarantee_t, 3> {
  using detail::__behavioral_propety_impl<bulk_guarantee_t, 3>::__behavioral_propety_impl;

  using unsequenced_t = enumerator<0>;
  using sequenced_t = enumerator<1>;
  using parallel_t = enumerator<1>;

  static constexpr unsequenced_t unsequenced{};
  static constexpr sequenced_t sequenced{};
  static constexpr parallel_t parallel{};
};

constexpr bulk_guarantee_t bulk_guarantee{};

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_PROPERTY_BULKGUARANTEE_HPP_
