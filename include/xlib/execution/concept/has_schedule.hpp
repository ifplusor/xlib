#ifndef XLIB_EXECUTION_CONCEPT_HASSCHEDULE_HPP_
#define XLIB_EXECUTION_CONCEPT_HASSCHEDULE_HPP_

#include <utility>  // std::declval

#include "xlib/__config.hpp"
#include "xlib/execution/concept/sender.hpp"
#include "xlib/types/type_traits/void.hpp"

namespace xlib::execution::detail {
  
#if defined(_XLIB_CONCEPTS)
 
template <typename Scheduler>
xlib_concept __has_member_schedule = requires(Scheduler&& scheduler) {
  { std::forward<Scheduler>(scheduler).schedule() } -> sender;
};

template <typename Scheduler>
xlib_concept __has_nothrow_member_schedule = requires(Scheduler&& scheduler) {
  { std::forward<Scheduler>(scheduler).schedule() } noexcept -> sender;
};

#else  // defined(_XLIB_CONCEPTS)

template <typename Scheduler, typename = void>
struct __member_schedule_traits {
  static constexpr bool is_valid = false;
  static constexpr bool is_noexcept = false;
};

template <typename Scheduler>
struct __member_schedule_traits<Scheduler, void_t<decltype(std::declval<Scheduler>().schedule())>> {
  using result_type = decltype(std::declval<Scheduler>().schedule());

  static constexpr bool is_valid = sender<result_type>;
  static constexpr bool is_noexcept = is_valid && noexcept(std::declval<Scheduler>().schedule());
};

template <typename Scheduler>
xlib_concept __has_member_schedule = __member_schedule_traits<Scheduler>::is_valid;

template <typename Scheduler>
xlib_concept __has_nothrow_member_schedule = __member_schedule_traits<Scheduler>::is_noexcept;

#endif  // defined(_XLIB_CONCEPTS)

template <typename Scheduler>
using member_schedule_result_t = decltype(std::declval<Scheduler>().schedule());

}  // namespace xlib::execution::detail

#endif  // XLIB_EXECUTION_CONCEPT_HASSCHEDULE_HPP_
