#ifndef XLIB_CONCEPTS_INVOCABLE_HPP_
#define XLIB_CONCEPTS_INVOCABLE_HPP_

#include "xlib/__config.hpp"

#if defined(_XLIB_LIB_CONCEPTS)

#include <concepts>

namespace xlib {

using std::invocable;

}

#else  // defined(_XLIB_LIB_CONCEPTS)

#include "xlib/concepts/concept.hpp"

#if defined(_XLIB_CONCEPTS)

#include <utility>

#include "xlib/utility/functional/invoke.hpp"

namespace xlib {

template <typename F, typename... Args>
xlib_concept invocable = requires(F&& f, Args&&... args) {
  invoke(std::forward<F>(f), std::forward<Args>(args)...);
  /* not required to be equality preserving */
};

}  // namespace xlib

#else  // defined(_XLIB_CONCEPTS)

#include <xlib/types/type_traits/is_invocable.hpp>

namespace xlib {

template <typename F, typename... Args>
xlib_concept invocable = xlib_is_invocable_v(F, Args...);

}  // namespace xlib

#endif  // defined(_XLIB_CONCEPTS)

#endif  // defined(_XLIB_LIB_CONCEPTS)

#endif  // XLIB_CONCEPTS_INVOCABLE_HPP_
