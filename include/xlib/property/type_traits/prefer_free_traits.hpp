#ifndef XLIB_PROPERTY_TYPETRAITS_PREFERFREETRAITS_HPP_
#define XLIB_PROPERTY_PREFERFREETRAITS_HPP_

#include <utility>  // std::declval

#include "xlib/types/type_traits/void.hpp"

namespace xlib::detail {

template <typename Entity, typename Property, typename = void_t<>>
struct __prefer_free_traits {
  static constexpr bool is_valid = false;
  static constexpr bool is_noexcept = false;
};

template <typename Entity, typename Property>
struct __prefer_free_traits<Entity,
                            Property,
                            void_t<decltype(prefer(std::declval<Entity>(), std::declval<Property>()))>> {
  using result_type = decltype(prefer(std::declval<Entity>(), std::declval<Property>()));

  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = noexcept(prefer(std::declval<Entity>(), std::declval<Property>()));
};

}  // namespace xlib::detail

#endif  // XLIB_PROPERTY_PREFERFREETRAITS_HPP_
