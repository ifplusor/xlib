#ifndef XLIB_CONCEPTS_SUPPORT_SFINAE_HPP_
#define XLIB_CONCEPTS_SUPPORT_SFINAE_HPP_

#include "xlib/concepts/support/concept.hpp"
#include "xlib/preprocessor/facilities/at.hpp"
#include "xlib/preprocessor/facilities/cat.hpp"
#include "xlib/preprocessor/facilities/check.hpp"
#include "xlib/preprocessor/facilities/eval.hpp"
#include "xlib/preprocessor/facilities/probe.hpp"
#include "xlib/preprocessor/logical/bool.hpp"
#include "xlib/types/type_traits/enable_if.hpp"
#include "xlib/types/type_traits/is_void.hpp"
#include "xlib/types/type_traits/void.hpp"

//
// template

#define _XLIB_TEMPLATE_SFINAE(...) template <__VA_ARGS__ _XLIB_TEMPLATE_SFINAE_AUX

#define _XLIB_TEMPLATE_SFINAE_AUX(...) \
  XLIB_PP_CAT(_XLIB_TEMPLATE_SFINAE_AUX_, _XLIB_TEMPLATE_SFINAE_AUX_WHICH(__VA_ARGS__, ))(__VA_ARGS__)

// Check whether to define a concept or use requires
#define _XLIB_TEMPLATE_SFINAE_AUX_WHICH(FIRST, ...) \
  XLIB_PP_EVAL(XLIB_PP_CHECK, XLIB_PP_CAT(_XLIB_TEMPLATE_SFINAE_PROBE_CONCEPT_, FIRST))
#define _XLIB_TEMPLATE_SFINAE_PROBE_CONCEPT_concept XLIB_PP_PROBE(~)

// A template with a requires clause
#define _XLIB_TEMPLATE_SFINAE_AUX_0(...)                                                                              \
  , bool _XLIB_true = true                                                                                            \
  , ::xlib::enable_if_t<XLIB_PP_CAT(_XLIB_TEMPLATE_SFINAE_AUX_3_, __VA_ARGS__) && XLIB_PP_BOOL(_XLIB_true), int> = 0>
#define _XLIB_TEMPLATE_SFINAE_AUX_3_requires

//
// concept

// A concept definition
#define _XLIB_TEMPLATE_SFINAE_AUX_1(DECL, ...)                                                               \
  , bool _XLIB_true = true, ::xlib::enable_if_t<__VA_ARGS__ && XLIB_PP_BOOL(_XLIB_true), int> = 0>           \
  auto _XLIB_CONCEPT_NAME(DECL)(::xlib::concepts::detail::__tag<_XLIB_CONCEPT_PARAMS(DECL)>*) -> char(&)[1]; \
                                                                                                             \
  auto _XLIB_CONCEPT_NAME(DECL)(...) -> char(&)[2]

#define _XLIB_CONCEPT_TYPENAME_SFINAE(...) \
  xlib_is_void_v(::xlib::void_t<__VA_ARGS__>)

/*
#define _XLIB_CONCEPT_TYPENAME_SFINAE(TYPENAME) \
  xlib_is_void_v(::xlib::void_t<_XLIB_CONCEPT_TYPENAME_SFINAE_##TYPENAME>)

#define _XLIB_CONCEPT_TYPENAME_SFINAE_(...) __VA_ARGS__
#define _XLIB_CONCEPT_TYPENAME_SFINAE_typename(...) typename __VA_ARGS__
*/

#define _XLIB_CONCEPT_AND_SFINAE \
  && XLIB_PP_BOOL(_XLIB_true), int> = 0, ::xlib::enable_if_t<

// NOTE: mark template arguments by tag
#define _XLIB_CONCEPT_REF_SFINAE(NAME, ...) \
  (1u ==                                    \
   sizeof(XLIB_PP_CAT(NAME, _concept_helper)(static_cast<::xlib::concepts::detail::__tag<__VA_ARGS__>*>(nullptr))))

//
// requires

#define _XLIB_REQUIRES_SFINAE(NAME, REQUIRES) \
  auto XLIB_PP_CAT(NAME, _requires_test_helper) _XLIB_REQUIRES_SFINAE_AUX(NAME, _XLIB_REQUIRES_SFINAE_##REQUIRES)

// A requires definition
#define _XLIB_REQUIRES_SFINAE_AUX(NAME, ...)                                                                     \
  __VA_ARGS__                                                                                                    \
                                                                                                                 \
  template <typename... As>                                                                                      \
  auto XLIB_PP_CAT(NAME, _requires_helper)(::xlib::concepts::detail::__tag<As...>*,                              \
                                           decltype(&XLIB_PP_CAT(NAME, _requires_test_helper)<As...>) = nullptr) \
      -> char(&)[1];                                                                                             \
                                                                                                                 \
  auto XLIB_PP_CAT(NAME, _requires_helper)(...) -> char(&)[2]

#define _XLIB_REQUIRES_SFINAE_requires(...) (__VA_ARGS__)->decltype _XLIB_REQUIRES_SFINAE_RETURN

#define _XLIB_REQUIRES_SFINAE_RETURN(...) \
  (__VA_ARGS__, void()) {}

// NOTE: mark template arguments by tag
#define _XLIB_REQUIRES_REF_SFINAE(NAME, ...) \
  (1u ==                                     \
   sizeof(XLIB_PP_CAT(NAME, _requires_helper)(static_cast<::xlib::concepts::detail::__tag<__VA_ARGS__>*>(nullptr))))

namespace xlib {
namespace concepts {
namespace detail {

template <typename...>
struct __tag;

}  // namespace detail
}  // namespace concepts
}  // namespace xlib

#endif  // XLIB_CONCEPTS_SUPPORT_SFINAE_HPP_
