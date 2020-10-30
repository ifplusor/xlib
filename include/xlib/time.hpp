#ifndef XLIB_TIME_HPP
#define XLIB_TIME_HPP

#include <chrono>

namespace xlib {

enum class time_unit { nanoseconds, microseconds, milliseconds, seconds, minutes, hours };

inline std::chrono::steady_clock::time_point until_time_point(long delay, time_unit unit) {
  auto now = std::chrono::steady_clock::now();
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

#endif  // XLIB_TIME_HPP
