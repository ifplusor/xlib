#ifndef XLIB_EXECUTION_CONCEPT_HASSUBMIT_HPP_
#define XLIB_EXECUTION_CONCEPT_HASSUBMIT_HPP_

#include <utility>  // std::declval, std::forward

#include "xlib/concepts/concept.hpp"
#include "xlib/types/type_traits/void.hpp"

namespace xlib::execution::detail {

#if defined(_XLIB_CONCEPTS)

template <typename Sender, typename Receiver>
xlib_concept __has_member_submit = requires(Sender&& sender, Receiver&& receiver) {
  std::forward<Sender>(sender).submit(std::forward<Receiver>(receiver));
};

template <typename Sender, typename Receiver>
xlib_concept __has_nothrow_member_submit = requires(Sender&& sender, Receiver&& receiver) {
  { std::forward<Sender>(sender).submit(std::forward<Receiver>(receiver)) } noexcept;
};

#else  // defined(_XLIB_CONCEPTS)

template <typename Sender, typename Receiver, typename = void>
struct __member_submit_traits {
  static constexpr bool is_valid = false;
  static constexpr bool is_noexcept = false;
};

template <typename Sender, typename Receiver>
struct __member_submit_traits<Sender,
                              Receiver,
                              void_t<decltype(std::declval<Sender>().submit(std::declval<Receiver>()))>> {
  using result_type = decltype(std::declval<Sender>().connect(std::declval<Receiver>()));

  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = noexcept(std::declval<Sender>().submit(std::declval<Receiver>()));
};

template <typename Sender, typename Receiver>
xlib_concept __has_member_submit = __member_submit_traits<Sender, Receiver>::is_valid;

template <typename Sender, typename Receiver>
xlib_concept __has_nothrow_member_submit = __member_submit_traits<Sender, Receiver>::is_noexcept;

#endif  // defined(_XLIB_CONCEPTS)

}  // namespace xlib::execution::detail

#endif  // XLIB_EXECUTION_CONCEPT_HASSUBMIT_HPP_
