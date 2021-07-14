#ifndef XLIB_EXECUTION_PROPERTY_OUTSTANDINGWORK_HPP_
#define XLIB_EXECUTION_PROPERTY_OUTSTANDINGWORK_HPP_

#include "xlib/execution/property/behavioral_property.hpp"

namespace xlib::execution {

/**
 * @brief Allows users of executors to indicate that task submission is likely in the future.
 */
struct outstanding_work_t : detail::__behavioral_propety_impl<outstanding_work_t, 2> {
  using detail::__behavioral_propety_impl<outstanding_work_t, 2>::__behavioral_propety_impl;

  using untracked_t = enumerator<0>;
  using tracked_t = enumerator<1>;

  static constexpr untracked_t untracked{};
  static constexpr tracked_t tracked{};
};

constexpr outstanding_work_t outstanding_work{};

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_PROPERTY_OUTSTANDINGWORK_HPP_
