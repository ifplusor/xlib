#ifndef XLIB_UTILITY_FUNCTIONAL_INVOKE_HPP_
#define XLIB_UTILITY_FUNCTIONAL_INVOKE_HPP_

#include <functional>

#include "xlib/__config.hpp"

#if _XLIB_STD_VER >= 2017L

namespace xlib {

using std::invoke;

}  // namespace xlib

#else  // _XLIB_STD_VER >= 2017L

#include <utility>  // std::forward

#include "xlib/utility/functional/invoke_op.hpp"
#include "xlib/types/type_traits/invoke_result.hpp"

namespace xlib {

template <class F, class... Args>
constexpr invoke_result_t<F, Args...> invoke(F&& f, Args&&... args)
// noexcept(is_nothrow_invocable_v<F, Args...>)
{
  return detail::INVOKE(std::forward<F>(f), std::forward<Args>(args)...);
}

}  // namespace xlib

#endif  // _XLIB_STD_VER >= 2017L

#endif  // XLIB_UTILITY_FUNCTIONAL_INVOKE_HPP_
