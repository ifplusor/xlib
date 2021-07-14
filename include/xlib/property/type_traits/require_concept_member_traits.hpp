#ifndef XLIB_PROPERTY_REQUIRECONCEPTMEMBERTRAITS_HPP_
#define XLIB_PROPERTY_REQUIRECONCEPTMEMBERTRAITS_HPP_

#include <utility>  // std::declval

#include "xlib/types/type_traits/void.hpp"

namespace xlib {

template <typename Entity, typename Property, typename = void_t<>>
struct require_concept_member_traits {
  static constexpr bool is_valid = false;
  static constexpr bool is_noexcept = false;
};

template <typename Entity, typename Property>
struct require_concept_member_traits<
    Entity,
    Property,
    void_t<decltype(std::declval<Entity>().require_concept(std::declval<Property>()))>> {
  using result_type = decltype(std::declval<Entity>().require_concept(std::declval<Property>()));

  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = noexcept(std::declval<Entity>().require_concept(std::declval<Property>()));
};

}  // namespace xlib

#endif  // XLIB_PROPERTY_REQUIRECONCEPTMEMBERTRAITS_HPP_
