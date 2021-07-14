#ifndef XLIB_EXECUTION_PROPERTY_CONTEXT_HPP_
#define XLIB_EXECUTION_PROPERTY_CONTEXT_HPP_

#include <any>
#include <utility>  // std::declval

#include "xlib/execution/concept/executor.hpp"

namespace xlib::execution {

namespace detail {

template <typename Derived>
struct __context_property_impl {
  template <typename T>
  static constexpr bool is_applicable_property_v = executor<T>;

  static constexpr bool is_requirable = false;
  static constexpr bool is_preferable = false;
  using polymorphic_query_result_type = std::any;

  template <typename Executor, typename Type = decltype(Executor::query(std::declval<Derived>()))>
  static constexpr Type static_query_v = Executor::query(Derived());
};

}  // namespace detail

/**
 * The context_t property can be used only with query, which returns the execution context associated with the executor.
 */
struct context_t : detail::__context_property_impl<context_t> {};

constexpr context_t context{};

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_PROPERTY_CONTEXT_HPP_
