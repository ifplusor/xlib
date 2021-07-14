#ifndef XLIB_PROPERTY_CONCEPT_HASQUERY_HPP_
#define XLIB_PROPERTY_CONCEPT_HASQUERY_HPP_

#include <utility>  // std::declval, std::forward

#include "xlib/concepts/concept.hpp"
#include "xlib/types/type_traits/void.hpp"

namespace xlib::detail {

#if defined(_XLIB_CONCEPTS)

template <typename Entity, typename Property>
xlib_concept __has_member_query = requires(Entity&& entity, Property&& property) {
  std::forward<Entity>(entity).query(std::forward<Property>(property));
};

template <typename Entity, typename Property>
xlib_concept __has_nothrow_member_query = requires(Entity&& entity, Property&& property) {
  { std::forward<Entity>(entity).query(std::forward<Property>(property)) } noexcept;
};

#else  // defined(_XLIB_CONCEPTS)

template <typename Entity, typename Property, typename = void_t<>>
struct __member_query_traits {
  static constexpr bool is_valid = false;
  static constexpr bool is_noexcept = false;
};

template <typename Entity, typename Property>
struct __member_query_traits<Entity,
                             Property,
                             void_t<decltype(std::declval<Entity>().query(std::declval<Property>()))>> {
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = noexcept(std::declval<Entity>().query(std::declval<Property>()));
};

template <typename Entity, typename Property>
xlib_concept __has_member_query = __member_query_traits<Entity, Property>::is_valid;

template <typename Entity, typename Property>
xlib_concept __has_nothrow_member_query = __member_query_traits<Entity, Property>::is_noexcept;

#endif  // defined(_XLIB_CONCEPTS)

template <typename Entity, typename Property>
using member_query_return_t = decltype(std::declval<Entity>().query(std::declval<Property>()));

}  // namespace xlib::detail

#endif  // XLIB_PROPERTY_CONCEPT_HASQUERY_HPP_
