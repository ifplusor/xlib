#ifndef XLIB_CONCEPTS_SUPPORT_NORMAL_HPP_
#define XLIB_CONCEPTS_SUPPORT_NORMAL_HPP_

#include "xlib/concepts/support/concept.hpp"
#include "xlib/preprocessor/facilities/cat.hpp"
#include "xlib/preprocessor/facilities/check.hpp"
#include "xlib/preprocessor/facilities/eval.hpp"

#define _XLIB_TEMPLATE_NORMAL(...) template <__VA_ARGS__ _XLIB_TEMPLATE_NORMAL_AUX

#define _XLIB_TEMPLATE_NORMAL_AUX(...) \
  > XLIB_PP_CAT(_XLIB_TEMPLATE_AUX_, _XLIB_TEMPLATE_NORMAL_AUX_WHICH(__VA_ARGS__, ))(__VA_ARGS__)

#define _XLIB_TEMPLATE_NORMAL_AUX_WHICH(FIRST, ...) \
  XLIB_PP_EVAL(XLIB_PP_CHECK, XLIB_PP_CAT(_XLIB_TEMPLATE_NORMAL_PROBE_CONCEPT_, FIRST))
#define _XLIB_TEMPLATE_NORMAL_PROBE_CONCEPT_concept XLIB_PP_PROBE(~)

// A template with a requires clause
#define _XLIB_TEMPLATE_NORMAL_AUX_0(...) __VA_ARGS__

// A concept definition
#define _XLIB_TEMPLATE_NORMAL_AUX_1(DECL, ...) xlib_concept _XLIB_CONCEPT_NAME_(DECL) = __VA_ARGS_

#endif  // XLIB_CONCEPTS_SUPPORT_NORMAL_HPP_
