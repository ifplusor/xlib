#ifndef XLIB_CONCEPTS_CONCEPT_HPP_
#define XLIB_CONCEPTS_CONCEPT_HPP_

/*
Usage:

  template <typename A, typename B>
  xlib_requires(RequiresFoo,
    requires(From (&from)())  //
    (
      static_cast<To>(from())
    ));

  template <typename A, typename B>
  xlib_concept ConceptBar = xlib_requires_ref(RequiresFoo, A, B);

  xlib_template(typename A, typename B, typename C)
    (requires ConceptBar<A, B> xlib_concept_and ConceptBaz<C>)
  void foobar(A a, B b, C c) {}

*/

#include "xlib/__config.hpp"

#if defined(_XLIB_CONCEPTS)

#include "xlib/concepts/support/normal.hpp"

#define xlib_template _XLIB_TEMPLATE_NORMAL

#else  // defined(_XLIB_CONCEPTS)

#include "xlib/concepts/support/sfinae.hpp"

#define xlib_template _XLIB_TEMPLATE_SFINAE

#define xlib_concept_typename _XLIB_CONCEPT_TYPENAME_SFINAE
#define xlib_concept_and _XLIB_CONCEPT_AND_SFINAE
#define xlib_concept_ref _XLIB_CONCEPT_REF_SFINAE

#define xlib_requires _XLIB_REQUIRES_SFINAE
#define xlib_requires_ref _XLIB_REQUIRES_REF_SFINAE

#endif  // defined(_XLIB_CONCEPTS)

/*
#if defined(_XLIB_CONCEPTS)

#define xlib_requires_t(require, ...) requires require __VA_ARGS__
#define xlib_requires_st(specifier, require, ...) requires require specifier __VA_ARGS__

#define XLIB_TEMPLATE(require, ...) \
  template <__VA_ARGS__>            \
  requires require
#define XLIB_TEMPLATE0(require) XLIB_TEMPLATE(require)

#define XLIB_REQUIRES_N(...) requires __VA_ARGS__
#define XLIB_REQUIRES_SN(specifier, ...) require __VA_ARGS__ specifier

#else

#include "xlib/types/type_traits/enable_if.hpp"

#define xlib_requires_t(...) ::xlib::enable_if_t<__VA_ARGS__>
#define xlib_requires_st(specifier, ...) specifier ::xlib::enable_if_t<__VA_ARGS__>

#define XLIB_TEMPLATE(require, ...) template <__VA_ARGS__, typename = enable_if_t<require>>
#define XLIB_TEMPLATE0(require) template <typename = enable_if_t<require>>

#define XLIB_REQUIRES_N(...) static_assert(__VA_ARGS__)
#define XLIB_REQUIRES_SN(specifier, ...) static_assert(__VA_ARGS__) specifier

#endif
*/

#endif  // XLIB_CONCEPTS_CONCEPT_HPP_
