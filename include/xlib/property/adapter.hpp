#ifndef XLIB_PROPERTY_ADAPTER_HPP_
#define XLIB_PROPERTY_ADAPTER_HPP_

#include <utility>  // std::move

#include "xlib/property/concept/has_query.hpp"
#include "xlib/property/type_traits/query_static_member_traits.hpp"
#include "xlib/property/type_traits/require_concept_member_traits.hpp"
#include "xlib/property/type_traits/require_member_traits.hpp"
#include "xlib/types/type_traits/enable_if.hpp"

namespace xlib {

template <template <typename> typename Derived, typename T>
class adapter {
 public:
  adapter(T v) : v_(std::move(v)) {}

  template <typename Property>
  constexpr auto require_concept(const Property& p) const
      noexcept(require_concept_member_traits<T, Property>::is_noexcept)
          -> Derived<typename require_concept_member_traits<T, Property>::result_type> {
    return Derived<decltype(v_.require_concept(p))>(v_.require_concept(p));
  }

  template <typename Property>
  constexpr auto require(const Property& p) const noexcept(detail::__require_member_traits<T, Property>::is_noexcept)
      -> Derived<typename detail::__require_member_traits<T, Property>::result_type> {
    return Derived<decltype(v_.require(p))>(v_.require(p));
  }

  template <typename Property>
  static constexpr auto query(const Property& p) noexcept(
      detail::__query_static_member_traits<T, Property>::is_noexcept) ->
      typename detail::__query_static_member_traits<T, Property>::result_type {
    return T::query(p);
  }

  template <typename Property>
  constexpr auto query(const Property& p) const noexcept(detail::__has_nothrow_member_query<T, Property>)
      -> enable_if_t<!detail::__query_static_member_traits<T, Property>::is_valid,
                     typename detail::member_query_return_t<T, Property>> {
    return v_.query(p);
  }

  friend constexpr bool operator==(const Derived<T>& a, const Derived<T>& b) noexcept {
    return a.executor_ == b.executor_;
  }

  friend constexpr bool operator!=(const Derived<T>& a, const Derived<T>& b) noexcept {
    return a.executor_ != b.executor_;
  }

 protected:
  T v_;
};

}  // namespace xlib

#endif  // XLIB_PROPERTY_ADAPTER_HPP_
