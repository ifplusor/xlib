#ifndef XLIB_PROPERTY_REQUIRE_HPP_
#define XLIB_PROPERTY_REQUIRE_HPP_

#include <utility>  // std::forward, std::declval

#include "xlib/__config.hpp"
#include "xlib/property/is_applicable_property.hpp"
#include "xlib/property/type_traits/require_free_traits.hpp"
#include "xlib/property/type_traits/require_member_traits.hpp"
#include "xlib/property/type_traits/require_static_traits.hpp"
#include "xlib/types/type_traits/decay.hpp"
#include "xlib/types/type_traits/enable_if.hpp"

namespace xlib {

namespace detail {

/**
 * @details
 * The expression `require(E, P0, Pn...)` for some subexpressions `E` and `P0`, and where `Pn...` represents `N`
 * subexpressions (where `N` is 0 or more, and with types `T = decay_t<decltype(E)>` and
 * `Prop0 = decay_t<decltype(P0)>`) is expression-equivalent to:
 *
 * - If `is_applicable_property_v<T, Prop0> && Prop0::is_requirable` is not a well-formed constant expression with value
 *   true, `std::require(E, P0, Pn...)` is ill-formed.
 */
struct __require_function {
  /**
   * @details
   * - Otherwise, `E` if `N == 0` and the expression `Prop0::template static_query_v<T> == Prop0::value()` is a
   *   well-formed constant expression with value true.
   *
   * @tparam Entity
   * @tparam Property
   * @param ex
   * @return __require_static_traits<Entity, Property>::result_type
   */
  template <typename Entity, typename Property>
  constexpr auto operator()(Entity&& ex, Property&& /*unused*/) const
      -> enable_if_t<is_applicable_property_v<decay_t<Entity>, decay_t<Property>> && decay_t<Property>::is_requirable &&
                         __require_static_traits<Entity, Property>::is_valid,
                     typename __require_static_traits<Entity, Property>::result_type> {
    return std::forward<Entity>(ex);
  }

  /**
   * @details
   * - Otherwise, `(E).require(P0)` if `N == 0` and the expression `(E).require(P0)` is a valid expression.
   *
   * @tparam Entity
   * @tparam Property
   * @param ex
   * @param p
   * @return __require_member_traits<Entity, Property>::result_type
   */
  template <typename Entity, typename Property>
  constexpr auto operator()(Entity&& ex, Property&& p) const
      noexcept(__require_member_traits<Entity, Property>::is_noexcept)
          -> enable_if_t<is_applicable_property_v<decay_t<Entity>, decay_t<Property>> &&
                             decay_t<Property>::is_requirable && !__require_static_traits<Entity, Property>::is_valid &&
                             __require_member_traits<Entity, Property>::is_valid,
                         typename __require_member_traits<Entity, Property>::result_type> {
    return std::forward<Entity>(ex).require(std::forward<Property>(p));
  }

  /**
   * @details
   * - Otherwise, `require(E, P0)` if `N == 0` and the expression `require(E, P0)` is a valid expression with overload
   *   resolution performed in a context that does not include the declaration of the require customization point
   *   object.
   *
   * @tparam Entity
   * @tparam Property
   * @param ex
   * @param p
   * @return __require_free_traits<Entity, Property>::result_type
   */
  template <typename Entity, typename Property>
  constexpr auto operator()(Entity&& ex, Property&& p) const
      noexcept(__require_free_traits<Entity, Property>::is_noexcept)
          -> enable_if_t<is_applicable_property_v<decay_t<Entity>, decay_t<Property>> &&
                             decay_t<Property>::is_requirable && !__require_static_traits<Entity, Property>::is_valid &&
                             !__require_member_traits<Entity, Property>::is_valid &&
                             __require_free_traits<Entity, Property>::is_valid,
                         typename __require_free_traits<Entity, Property>::result_type> {
    return require(std::forward<Entity>(ex), std::forward<Property>(p));
  }

  /**
   * @details
   * - Otherwise, `std::require(std::require(E, P0), Pn...)` if `N > 0` and the expression
   *   `std::require(std::require(E, P0), Pn...)` is a valid expression.
   *
   * @tparam Entity
   * @tparam Property0
   * @tparam Property1
   * @tparam PropertyN
   * @param ex
   * @param p0
   * @param p1
   * @param pn
   * @return auto
   */
  template <typename Entity, typename Property0, typename Property1, typename... PropertyN>
  constexpr auto operator()(Entity&& ex, Property0&& p0, Property1&& p1, PropertyN&&... pn) const noexcept(
      noexcept(std::declval<__require_function>()(std::declval<__require_function>()(std::forward<Entity>(ex),
                                                                                     std::forward<Property0>(p0)),
                                                  std::forward<Property1>(p1),
                                                  std::forward<PropertyN>(pn)...)))
      -> decltype(std::declval<__require_function>()(std::declval<__require_function>()(std::forward<Entity>(ex),
                                                                                        std::forward<Property0>(p0)),
                                                     std::forward<Property1>(p1),
                                                     std::forward<PropertyN>(pn)...)) {
    return (*this)((*this)(std::forward<Entity>(ex), std::forward<Property0>(p0)), std::forward<Property1>(p1),
                   std::forward<PropertyN>(pn)...);
  }

  /**
   * - Otherwise, std::require(E, P0, Pn...) is ill-formed.
   */
};

template <typename T = __require_function>
_XLIB_INLINE_VAR constexpr T __require_customization_point{};

}  // namespace detail

inline namespace cpos {

/**
 * @brief The name `require` denotes a customization point object.
 */
_XLIB_INLINE_VAR constexpr const auto& require = detail::__require_customization_point<>;

}  // namespace cpos

}  // namespace xlib

#endif  // XLIB_PROPERTY_REQUIRE_HPP_
