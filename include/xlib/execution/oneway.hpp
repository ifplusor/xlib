#ifndef XLIB_EXECUTION_ONEWAY_HPP_
#define XLIB_EXECUTION_ONEWAY_HPP_

namespace xlib {

namespace execution {

struct oneway_t {
  static constexpr bool is_requirable_concept = true;
  static constexpr bool is_requirable = false;
  static constexpr bool is_preferable = false;
  using polymorphic_query_result_type = bool;

  template <class... SupportableProperties>
  class polymorphic_executor_type;

#if defined(_XLIB_CONCEPTS)
  template <class Executor>
  static constexpr bool static_query_v = OneWayExecutor<Executor>;
#else
  template <class Executor>
  static constexpr bool static_query_v = is_oneway_executor<Executor>::value;
#endif

  static constexpr bool value() { return true; }
};

constexpr oneway_t oneway;

}  // namespace execution

}  // namespace xlib

#endif  // XLIB_EXECUTION_ONEWAY_HPP_
