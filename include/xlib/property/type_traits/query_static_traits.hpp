#ifndef XLIB_PROPERTY_QUERYSTATICTRAITS_HPP_
#define XLIB_PROPERTY_QUERYSTATICTRAITS_HPP_

#include "xlib/types/type_traits/decay.hpp"
#include "xlib/types/type_traits/void.hpp"

namespace xlib::detail {

template <typename Entity, typename Property, typename = void_t<>>
struct __query_static_traits {
  static constexpr bool is_valid = false;
  static constexpr bool is_noexcept = false;
};

template <typename Entity, typename Property>
struct __query_static_traits<Entity,
                             Property,
                             void_t<decltype(decay_t<Property>::template static_query_v<decay_t<Entity>>)>> {
  using result_type = decltype(decay_t<Property>::template static_query_v<decay_t<Entity>>);

  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = true;
};

}  // namespace xlib::detail

#endif  // XLIB_PROPERTY_QUERYSTATICTRAITS_HPP_
