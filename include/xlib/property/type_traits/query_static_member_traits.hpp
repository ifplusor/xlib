#ifndef XLIB_PROPERTY_QUERYSTATICMEMBERTRAITS_HPP_
#define XLIB_PROPERTY_QUERYSTATICMEMBERTRAITS_HPP_

#include <utility>  // std::declval

#include "xlib/types/type_traits/decay.hpp"
#include "xlib/types/type_traits/void.hpp"

namespace xlib::detail {

template <typename Entity, typename Property, typename = void_t<>>
struct __query_static_member_traits {
  static constexpr bool is_valid = false;
  static constexpr bool is_noexcept = false;
};

template <typename Entity, typename Property>
struct __query_static_member_traits<Entity,
                                    Property,
                                    void_t<decltype(decay_t<Entity>::query(std::declval<Property>()))>> {
  using result_type = decltype(decay_t<Entity>::query(std::declval<Property>()));

  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = noexcept(decay_t<Entity>::query(std::declval<Property>()));
};

}  // namespace xlib::detail

#endif  // XLIB_PROPERTY_QUERYSTATICMEMBERTRAITS_HPP_
