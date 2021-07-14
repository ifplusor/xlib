#ifndef XLIB_PROPERTY_TYPETRAITS_REQUIRESTATICTRAITS_HPP_
#define XLIB_PROPERTY_TYPETRAITS_REQUIRESTATICTRAITS_HPP_

#include "xlib/types/type_traits/decay.hpp"
#include "xlib/types/type_traits/enable_if.hpp"
#include "xlib/types/type_traits/void.hpp"

namespace xlib::detail {

template <typename Entity, typename Property, typename = void_t<>>
struct __require_static_traits {
  static constexpr bool is_valid = false;
  static constexpr bool is_noexcept = false;
};

template <typename Entity, typename Property>
struct __require_static_traits<
    Entity,
    Property,
    enable_if_t<decay_t<Property>::value() == decay_t<Property>::template static_query_v<decay_t<Entity>>>> {
  using result_type = decay_t<Entity>;

  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = noexcept(result_type(std::declval<Entity>()));
};

}  // namespace xlib::detail

#endif  // XLIB_PROPERTY_TYPETRAITS_REQUIRESTATICTRAITS_HPP_
