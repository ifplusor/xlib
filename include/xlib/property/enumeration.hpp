#ifndef XLIB_PROPERTY_ENUMERATION_HPP_
#define XLIB_PROPERTY_ENUMERATION_HPP_

#include <utility>  // std::declval

#include "xlib/property/can_query.hpp"
#include "xlib/property/concept/has_query.hpp"
#include "xlib/property/is_applicable_property.hpp"
#include "xlib/property/query.hpp"
#include "xlib/property/type_traits/query_static_traits.hpp"
#include "xlib/types/type_traits/conditional.hpp"
#include "xlib/types/type_traits/decay.hpp"
#include "xlib/types/type_traits/enable_if.hpp"
#include "xlib/types/type_traits/integral_constant.hpp"
#include "xlib/types/type_traits/is_same.hpp"

namespace xlib {

template <typename Enumeration, int N, int Value>
struct basic_enumerator;

namespace detail {

/**
 * @brief base type for default enumerator
 *
 * @tparam Enumeration
 * @tparam N The count of enumerator
 * @tparam Enumerator Enumeration::Enumerator<1>
 */
template <typename Enumeration, int N, typename Enumerator>
struct __default_enumerator {
  // T::query(Enumeration::Enumerator<1>()), if that expression is a well-formed expression;
  template <typename T>
  static constexpr auto static_query() -> decltype(T::query(std::declval<Enumerator>())) {
    return T::query(Enumerator());
  }

  template <typename T, int I = 1>
  struct use_default_static_query
      : conditional_t<can_query<T, basic_enumerator<Enumeration, N, I>>::value,
                      false_type,
                      conditional_t<I + 1 < N, use_default_static_query<T, I + 1>, true_type>> {};

  // ill-formed if declval<T().query(Enumeration::Enumerator<1>()) is well-formed;
  // ill-formed if can_query_v<T, Enumeration::Enumerator<I>()> is true for any 1 < i < N;
  template <typename T>
  static constexpr auto static_query()
      -> enable_if_t<!__has_member_query<T, Enumerator> && use_default_static_query<T>::value, Enumerator> {
    // otherwise Enumeration::Enumerator<1>().
    return Enumerator();
  }
};

/**
 * @brief base type for non default enumerator
 *
 * @tparam Enumeration
 * @tparam N The count of enumerator
 * @tparam Enumerator Enumeration::Enumerator<I>
 */
template <typename Enumeration, int N, typename Enumerator>
struct __non_default_enumerator {
  // T::query(Enumeration::Enumerator<I>()), if that expression is a well-formed expression;
  template <typename T>
  static constexpr auto static_query() -> decltype(T::query(std::declval<Enumerator>())) {
    return T::query(Enumerator());
  }
};

}  // namespace detail

template <typename Enumeration, int N, int Value>
struct basic_enumerator
    : conditional_t<Value == 0,
                    detail::__default_enumerator<Enumeration, N, basic_enumerator<Enumeration, N, Value>>,
                    detail::__non_default_enumerator<Enumeration, N, basic_enumerator<Enumeration, N, Value>>> {
  using base_type_helper =
      conditional_t<Value == 0,
                    detail::__default_enumerator<Enumeration, N, basic_enumerator<Enumeration, N, Value>>,
                    detail::__non_default_enumerator<Enumeration, N, basic_enumerator<Enumeration, N, Value>>>;

  template <typename T>
  static constexpr bool is_applicable_property_v = ::xlib::is_applicable_property<T, Enumeration>::value;

  // static constexpr bool is_requirable_concept = false;
  static constexpr bool is_requirable = true;
  static constexpr bool is_preferable = true;
  using polymorphic_query_result_type = Enumeration;

  template <typename T,
            typename Type = enable_if_t<(base_type_helper::template static_query<T>(), true),
                                        decltype(base_type_helper::template static_query<T>())>>
  static constexpr Type static_query_v = base_type_helper::template static_query<T>();

  static constexpr Enumeration value() { return Enumeration(Value); }
};

/**
 * @brief The base implementation of encumeration
 *
 * @tparam Enumeration
 * @tparam N The count of enumerator
 *
 * @see Section 2.2.12 Behavioral properties in https://wg21.link/P0443R14
 */
template <typename Enumeration, int N>
struct enumeration {
  /**
   * @brief The base implementation of enumerator
   *
   * @tparam I The index of enumerator
   */
  template <int I>
  using enumerator = basic_enumerator<Enumeration, N, I>;

  // template <typename T>
  // static constexpr bool is_applicable_property_v = executor<T>;

  // static constexpr bool is_requirable_concept = false;
  static constexpr bool is_requirable = false;
  static constexpr bool is_preferable = false;
  using polymorphic_query_result_type = Enumeration;

  // T::query(Enumeration()), if that expression is a well-formed constant expression;
  template <typename T>
  static constexpr auto static_query() -> decltype(T::query(std::declval<Enumeration>())) {
    return T::query(Enumeration());
  }

  template <typename T, int I = 0>
  struct static_query_type
      : conditional_t<detail::__query_static_traits<T, enumerator<I>>::is_valid,
                      decay<enumerator<I>>,
                      conditional_t<I + 1 < N, static_query_type<T, I + 1>, decay<enable_if<false>>>> {};

  // otherwise, ill-formed if declval<T>().query(Enumeration()) is well-formed;
  // otherwise, Enumeration::Enumerator<I>::static_query_v<T> for the least i < N for which this expression is a
  // well-formed constant expression;
  template <typename T>
  static constexpr auto static_query()
      -> enable_if_t<!detail::__has_member_query<T, Enumeration>,
                     decltype(static_query_type<T>::type::template static_query_v<T>)> {
    return static_query_type<T>::type::template static_query_v<T>;
  }

  template <typename T, typename Type = enable_if_t<(static_query<T>(), true), decltype(static_query<T>())>>
  static constexpr Type static_query_v = static_query<T>();

  template <typename Executor, int I = 0>
  struct query_type : conditional_t<can_query<Executor, enumerator<I>>::value,
                                    decay<enumerator<I>>,
                                    conditional_t<I + 1 < N, query_type<Executor, I + 1>, decay<enable_if<false>>>> {};

  template <typename T, typename Property, typename = enable_if_t<is_same<Property, Enumeration>::value>>
  friend constexpr auto query(const T& ex, const Property& /*unused*/) noexcept(
      noexcept(::xlib::query(ex, typename query_type<T>::type())))
      -> decltype(::xlib::query(ex, typename query_type<T>::type())) {
    return ::xlib::query(ex, typename query_type<T>::type());
  }

  friend constexpr bool operator==(const Enumeration& a, const Enumeration& b) noexcept { return a.value_ == b.value_; }
  friend constexpr bool operator!=(const Enumeration& a, const Enumeration& b) noexcept { return !operator==(a, b); }

  constexpr enumeration() = default;

  template <int I, typename = enable_if_t<(I >= 0 && I < N)>>
  constexpr enumeration(enumerator<I> /*unused*/) : value_(enumerator<I>::value().value_) {}

 private:
  template <typename, int, int>
  friend struct basic_enumerator;

  constexpr enumeration(int v) : value_(v) {}

  int value_{-1};
};

}  // namespace xlib

#endif  // XLIB_PROPERTY_ENUMERATION_HPP_
