#ifndef XLIB_EXECUTION_SUPPORT_INVOCABLEARCHETYPE_HPP_
#define XLIB_EXECUTION_SUPPORT_INVOCABLEARCHETYPE_HPP_

#include <functional>

namespace xlib::execution {

using invocable_archetype = std::function<void()>;

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_SUPPORT_INVOCABLEARCHETYPE_HPP_
