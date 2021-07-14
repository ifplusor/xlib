#ifndef XLIB_EXECUTION_TYPETRAITS_EXECUTORINDEX_HPP_
#define XLIB_EXECUTION_TYPETRAITS_EXECUTORINDEX_HPP_

#include <utility>  // std::declval

#include "xlib/property/require.hpp"
#include "xlib/execution/property/bulk_guarantee.hpp"
#include "xlib/execution/type_traits/executor_shape.hpp"
#include "xlib/types/type_traits/is_detected.hpp"
#include "xlib/types/type_traits/is_integral.hpp"

namespace xlib::execution {

template <typename Executor>
struct executor_index {
 private:
  // exposition only
  template <class T>
  using helper = typename T::index_type;

 public:
  using type = detected_or_t<executor_shape_t<Executor>,
                             helper,
                             decltype(::xlib::require(std::declval<const Executor&>(), bulk_guarantee))>;

  // exposition only
  static_assert(is_integral_v<type>, "index type must be an integral type");
};

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_TYPETRAITS_EXECUTORINDEX_HPP_
