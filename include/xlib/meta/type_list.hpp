#ifndef XLIB_META_TYPELIST_HPP_
#define XLIB_META_TYPELIST_HPP_

#include "xlib/types/type_traits/integral_constant.hpp"
#include "xlib/utility/integer_sequence.hpp"

namespace xlib {

template <typename... Ts>
struct type_list;

namespace detail {

template <typename... TypeLists>
struct __concat_type_lists_impl;

template <>
struct __concat_type_lists_impl<> {
  using type = type_list<>;
};

template <typename TypeList>
struct __concat_type_lists_impl<TypeList> {
  using type = TypeList;
};

template <typename... Bs, typename... Es>
struct __concat_type_lists_impl<type_list<Bs...>, type_list<Es...>> {
  using type = type_list<Bs..., Es...>;
};

template <typename... Bs, typename... Us, typename... Es, typename... OtherLists>
struct __concat_type_lists_impl<type_list<Bs...>, type_list<Us...>, type_list<Es...>, OtherLists...>
    : __concat_type_lists_impl<type_list<Bs..., Us..., Es...>, OtherLists...> {};

template <typename... Ts>
struct __type_list_helper {
  using size_type = index_sequence<>::value_type;

  template <size_type N>
  using index = integral_constant<size_type, N>;

  static constexpr size_type size() { return sizeof...(Ts); }

  template <size_type Index>
  struct __at_impl {
    template <size_type N, typename U, typename... Us>
    struct access_helper : access_helper<N + 1, Us...> {};

    template <typename U, typename... Us>
    struct access_helper<Index, U, Us...> {
      using type = U;
    };

    using type = typename access_helper<0, Ts...>::type;
  };

  template <size_type Index, typename U>
  struct __set_impl {
    template <size_type N>
    static auto at_helper(index<N>) -> typename __at_impl<N>::type;

    static auto at_helper(index<Index>) -> U;

    template <size_type... Ns>
    static auto set_helper(index_sequence<Ns...>) -> type_list<decltype(at_helper(index<Ns>{}))...>;

    using type = decltype(set_helper(make_index_sequence<__type_list_helper::size()>{}));
  };

  // FIXME
  // struct __pop_back_impl {
  //  template <size_type... Ns>
  //  static auto helper(index_sequence<Ns...>) -> type_list<typename __at_impl<Ns>::type...>;
  //
  //  using type = enable_if_t<__type_list_impl::size() != 0,
  //                             decltype(helper(make_index_sequence<__type_list_impl::size() - 1>{}))>;
  //};

  template <size_type Position, typename TypeList>
  struct __splice_impl;

  template <size_type Position, typename... Us>
  struct __splice_impl<Position, type_list<Us...>> {
    template <size_type... Ns>
    static auto at_helper(index_sequence<Ns...>) -> type_list<typename __at_impl<Ns>::type...>;

    template <size_type E>
    static auto at_begin_helper(index<E>) -> decltype(at_helper(make_index_sequence<E>{}));

    template <size_type B>
    static auto at_end_helper(index<B>)
        -> decltype(at_helper(index_sequence_begin<B, __type_list_helper::size() - B>{}));

    using type = typename __concat_type_lists_impl<decltype(at_begin_helper(index<Position>{})),
                                                   type_list<Us...>,
                                                   decltype(at_end_helper(index<Position>{}))>::type;
  };
};

}  // namespace detail

template <typename... Ts>
struct type_list {
  // Types:
  using size_type = typename detail::__type_list_helper<Ts...>::size_type;

  static constexpr size_type size() { return detail::__type_list_helper<Ts...>::size(); }

  template <size_type Index>
  using at = typename detail::__type_list_helper<Ts...>::template __at_impl<Index>::type;

  template <size_type Index, typename U>
  using set = typename detail::__type_list_helper<Ts...>::template __set_impl<Index, U>::type;

  template <typename... Us>
  using push_back = type_list<Ts..., Us...>;

  // using pop_back = typename detail::__type_list_helper<Ts...>::__pop_back_impl::type;

  template <size_type Position, typename... Us>
  using splice = typename detail::__type_list_helper<Ts...>::template __splice_impl<Position, Us...>::type;

  template <template <typename...> typename U>
  using apply = U<Ts...>;
};

template <typename... TypeLists>
using concat_type_lists_t = typename detail::__concat_type_lists_impl<TypeLists...>::type;

}  // namespace xlib

#endif  // XLIB_META_TYPELIST_HPP_
