#ifndef XLIB_TIME_HPP
#define XLIB_TIME_HPP

#include <chrono>

namespace xlib {

enum time_unit { nanoseconds, microseconds, milliseconds, seconds, minutes, hours };

inline std::chrono::steady_clock::time_point until_time_point(long delay, time_unit unit) {
  auto now = std::chrono::steady_clock::now();
  switch (unit) {
    case nanoseconds:
      return now + std::chrono::nanoseconds(delay);
    case microseconds:
      return now + std::chrono::microseconds(delay);
    case milliseconds:
      return now + std::chrono::milliseconds(delay);
    case seconds:
      return now + std::chrono::seconds(delay);
    case minutes:
      return now + std::chrono::minutes(delay);
    case hours:
      return now + std::chrono::hours(delay);
  }
}

}  // namespace xlib

#endif  // XLIB_TIME_HPP
