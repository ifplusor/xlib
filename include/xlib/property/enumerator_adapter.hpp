#ifndef XLIB_PROPERTY_ENUMERATORADAPTER_HPP_
#define XLIB_PROPERTY_ENUMERATORADAPTER_HPP_

#include <utility>  // std::declval

#include "xlib/property/adapter.hpp"
#include "xlib/property/require.hpp"

namespace xlib {

template <template <typename> typename Derived, typename T, typename Enumeration, typename Enumerator>
class enumerator_adapter : public adapter<Derived, T> {
 public:
  using adapter<Derived, T>::adapter;
  using adapter<Derived, T>::require;
  using adapter<Derived, T>::query;

  template <int N>
  constexpr auto require(const typename Enumeration::template enumerator<N>& p) const
      -> decltype(::xlib::require(std::declval<T>(), p)) {
    return ::xlib::require(this->v_, p);
  }

  static constexpr Enumeration query(const Enumeration&) noexcept { return Enumerator{}; }

  template <int N>
  static constexpr Enumeration query(const typename Enumeration::template enumerator<N>&) noexcept {
    return Enumerator{};
  }
};

}  // namespace xlib

#endif  // XLIB_PROPERTY_ENUMERATORADAPTER_HPP_
