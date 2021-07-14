#ifndef XLIB_UTILITY_INTEGERSEQUENCE_HPP_
#define XLIB_UTILITY_INTEGERSEQUENCE_HPP_

#include "xlib/__config.hpp"

#if _XLIB_STD_VER >= 2014L

#include <utility>

namespace xlib {

using std::integer_sequence;
using std::make_integer_sequence;

using std::index_sequence;
using std::make_index_sequence;

using std::index_sequence_for;

}  // namespace xlib

#else  // _XLIB_STD_VER >= 2014L

// TODO: implement integer_sequence

#endif  // _XLIB_STD_VER >= 2014L

namespace xlib {

namespace detail {

template <typename T, T Begin, typename IntegerSequence>
struct __integer_sequence_begin_impl;

template <typename T, T Begin, T... Ns>
struct __integer_sequence_begin_impl<T, Begin, integer_sequence<T, Ns...>> {
  using type = integer_sequence<T, (Begin + Ns)...>;
};

}  // namespace detail

template <typename T, T Begin, T N>
using integer_sequence_begin =
    typename detail::__integer_sequence_begin_impl<T, Begin, make_integer_sequence<T, N>>::type;

template <std::size_t Begin, std::size_t N>
using index_sequence_begin = integer_sequence_begin<std::size_t, Begin, N>;

}  // namespace xlib

#endif  // XLIB_UTILITY_INTEGERSEQUENCE_HPP_
