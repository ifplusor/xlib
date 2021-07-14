#ifndef XLIB_PROPERTY_QUERY_HPP_
#define XLIB_PROPERTY_QUERY_HPP_

#include "utility"

#include "xlib/__config.hpp"
#include "xlib/concepts/concept.hpp"
#include "xlib/property/concept/has_query.hpp"
#include "xlib/property/is_applicable_property.hpp"
#include "xlib/property/type_traits/query_free_traits.hpp"
#include "xlib/property/type_traits/query_static_traits.hpp"
#include "xlib/types/type_traits/decay.hpp"
#include "xlib/types/type_traits/enable_if.hpp"

namespace xlib {

namespace detail {

template <typename Entity, typename Property>
void query(Entity&&, Property&&) = delete;

/**
 * @details
 * The expression `std::query(E, P)` for some subexpressions `E` and `P` (with types `T = decay_t<decltype(E)>` and
 * `Prop = decay_t<decltype(P)>`) is expression-equivalent to:
 *
 * - If `is_applicable_property_v<T, Prop>` is not a well-formed constant expression with value true, `std::query(E, P)`
 *   is ill-formed.
 */
struct __query_function {
 private:
  template <typename Entity, typename Property>
  static constexpr int __query_dispatcher() {
    if constexpr (is_applicable_property_v<decay_t<Entity>, decay_t<Property>>) {
      if constexpr (__query_static_traits<Entity, Property>::is_valid) {
        return 1;
      } else if constexpr (__has_member_query<Entity, Property>) {
        return 2;
      } else if constexpr (__query_free_traits<Entity, Property>::is_valid) {
        return 3;
      }
    }
    return 0;
  }

 public:
  /**
   * @details
   * - Otherwise, `Prop::template static_query_v<T>` if the expression `Prop::template static_query_v<T>` is a
   *   well-formed constant expression.
   *
   * @tparam Entity
   * @tparam Property
   * @return __query_static_traits<Entity, Property>::result_type
   */
  template <typename Entity, typename Property>
  xlib_requires_st(constexpr,
                   (__query_dispatcher<Entity, Property>() == 1),
                   typename __query_static_traits<Entity, Property>::result_type)
  operator()(Entity&& /*unused*/, Property&& /*unused*/) const {
    return decay_t<Property>::template static_query_v<decay_t<Entity>>;
  }

  /**
   * @details
   * - Otherwise, `(E).query(P)` if the expression `(E).query(P)` is well-formed.
   *
   * @tparam Entity
   * @tparam Property
   * @param ex
   * @param p
   * @return __query_member_traits<Entity, Property>::result_type>
   */
  template <typename Entity, typename Property>
  xlib_requires_st(constexpr, (__query_dispatcher<Entity, Property>() == 2), member_query_return_t<Entity, Property>)
  operator()(Entity&& ex, Property&& p) const noexcept(__has_nothrow_member_query<Entity, Property>) {
    return std::forward<Entity>(ex).query(std::forward<Property>(p));
  }

  /**
   * @details
   * - Otherwise, `query(E, P)` if the expression `query(E, P)` is a valid expression with overload resolution performed
   *   in a context that does not include the declaration of the query customization point object.
   *
   * @tparam Entity
   * @tparam Property
   * @param ex
   * @param p
   * @return __query_free_traits<Entity, Property>::result_type
   */
  template <typename Entity, typename Property>
  xlib_requires_st(constexpr,
                   (__query_dispatcher<Entity, Property>() == 3),
                   typename __query_free_traits<Entity, Property>::result_type)
  operator()(Entity&& ex, Property&& p) const noexcept(__query_free_traits<Entity, Property>::is_noexcept) {
    return query(std::forward<Entity>(ex), std::forward<Property>(p));
  }

  /**
   * - Otherwise, std::query(E, P) is ill-formed.
   */
};

template <typename T = __query_function>
_XLIB_INLINE_VAR constexpr T __query_customization_point{};

}  // namespace detail

inline namespace cpos {

/**
 * @brief The name query denotes a customization point object.
 */
_XLIB_INLINE_VAR constexpr const auto& query = detail::__query_customization_point<>;

}  // namespace cpos

}  // namespace xlib

#endif  // XLIB_PROPERTY_QUERY_HPP_
