#ifndef XLIB_META_TYPESET_HPP_
#define XLIB_META_TYPESET_HPP_

#include "xlib/meta/type_list.hpp"
#include "xlib/types/type_traits/is_one_of.hpp"

namespace xlib {

template <typename... Ts>
struct type_set;

namespace detail {

template <typename... UniqueLists>
struct __concat_unique_lists_impl;

template <>
struct __concat_unique_lists_impl<> {
  using type = type_list<>;
};

template <typename UniqueList>
struct __concat_unique_lists_impl<UniqueList> {
  using type = UniqueList;
};

template <typename... Ts, typename... Us, typename... OtherLists>
struct __concat_unique_lists_impl<type_list<Ts...>, type_list<Us...>, OtherLists...>
    : __concat_unique_lists_impl<
          typename __concat_type_lists_impl<type_list<Ts...>,
                                            conditional_t<is_one_of_v<Us, Ts...>, type_list<>, type_list<Us>>...>::type,
          OtherLists...> {};

template <typename... Ts>
struct __type_set_helper {
  template <typename... TypeSets>
  struct __merge_impl;

  template <typename... Us>
  struct __merge_impl<type_set<Us...>> {
    using type = type_set<Ts..., Us...>;
  };

  template <typename... Us, typename... Vs, typename... OtherSets>
  struct __merge_impl<type_set<Us...>, type_set<Vs...>, OtherSets...>
      : __merge_impl<type_set<Us..., Vs...>, OtherSets...> {};
};

}  // namespace detail

template <typename... Ts>
struct type_set {
  using list_type = typename detail::__concat_unique_lists_impl<type_list<Ts>...>::type;

  template <typename U>
  static constexpr bool contains = is_one_of_v<U, Ts...>;

  template <typename... Us>
  using insert = type_set<Ts..., Us...>;

  template <typename... TypeSets>
  using merge = typename detail::__type_set_helper<Ts...>::template __merge_impl<TypeSets...>::type;
};

}  // namespace xlib

#endif  // XLIB_META_TYPESET_HPP_
