#ifndef XLIB_PROPERTY_CANQUERY_HPP_
#define XLIB_PROPERTY_CANQUERY_HPP_

#include <tuple>
#include <utility>

#include "xlib/property/query.hpp"
#include "xlib/types/type_traits/integral_constant.hpp"
#include "xlib/types/type_traits/void.hpp"

namespace xlib {

namespace detail {

template <typename Entity, typename Properties, typename = void_t<>>
struct __can_query : false_type {};

template <typename Entity, typename Property>
struct __can_query<Entity,
                   std::tuple<Property>,
                   void_t<decltype(::xlib::query(std::declval<Entity>(), std::declval<Property>()))>> : true_type {};

}  // namespace detail

/**
 * @brief Testing the ability of query Property for Entity
 *
 * @tparam Entity
 * @tparam Property
 */
template <typename Entity, typename Property>
struct can_query : detail::__can_query<Entity, std::tuple<Property>> {};

}  // namespace xlib

#endif  // XLIB_PROPERTY_CANQUERY_HPP_
