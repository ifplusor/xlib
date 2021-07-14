#ifndef XLIB_EXECUTION_TYPETRAITS_EXECUTORSHAPE_HPP_
#define XLIB_EXECUTION_TYPETRAITS_EXECUTORSHAPE_HPP_

#include <utility>  // std::declval

#include "xlib/property/require.hpp"
#include "xlib/execution/property/bulk_guarantee.hpp"
#include "xlib/types/type_traits/is_detected.hpp"
#include "xlib/types/type_traits/is_integral.hpp"

namespace xlib::execution {

template <typename Executor>
struct executor_shape {
 private:
  // exposition only
  template <class T>
  using helper = typename T::shape_type;

 public:
  using type =
      detected_or_t<size_t, helper, decltype(::xlib::require(std::declval<const Executor&>(), bulk_guarantee))>;

  // exposition only
  static_assert(is_integral_v<type>, "shape type must be an integral type");
};

template <typename Executor>
using executor_shape_t = typename executor_shape<Executor>::type;

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_TYPETRAITS_EXECUTORSHAPE_HPP_
