#ifndef XLIB_UTILITY_CHRONO_TIMEPOINT_HPP_
#define XLIB_UTILITY_CHRONO_TIMEPOINT_HPP_

#include <chrono>

#include "xlib/chrono/time_unit.hpp"

namespace xlib {

template <typename Clock = std::chrono::steady_clock>
inline typename Clock::time_point until_time_point(long delay, time_unit unit) {
  auto now = Clock::now();
  switch (unit) {
    case time_unit::nanoseconds:
      return now + std::chrono::nanoseconds(delay);
    case time_unit::microseconds:
      return now + std::chrono::microseconds(delay);
    case time_unit::milliseconds:
      return now + std::chrono::milliseconds(delay);
    case time_unit::seconds:
      return now + std::chrono::seconds(delay);
    case time_unit::minutes:
      return now + std::chrono::minutes(delay);
    case time_unit::hours:
      return now + std::chrono::hours(delay);
  }
}

}  // namespace xlib

#endif  // XLIB_UTILITY_CHRONO_TIMEPOINT_HPP_
