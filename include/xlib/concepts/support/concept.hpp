#ifndef XLIB_CONCEPTS_SUPPORT_CONCEPT_HPP_
#define XLIB_CONCEPTS_SUPPORT_CONCEPT_HPP_

#include "xlib/__config.hpp"
#include "xlib/preprocessor/facilities/at.hpp"
#include "xlib/preprocessor/facilities/cat.hpp"
#include "xlib/preprocessor/facilities/eval.hpp"
#include "xlib/preprocessor/facilities/ignore.hpp"

#if defined(_XLIB_CONCEPTS)
#define xlib_concept concept
#else
#define xlib_concept _XLIB_INLINE_VAR constexpr bool
#endif

#define _XLIB_CONCEPT_NAME(DECL) \
  XLIB_PP_EVAL(XLIB_PP_CAT, XLIB_PP_EVAL(XLIB_PP_FIRST, _XLIB_IGNORE_CONCEPT(DECL)), _concept_helper)

#define _XLIB_CONCEPT_PARAMS(DECL) XLIB_PP_EVAL(XLIB_PP_SECOND, _XLIB_IGNORE_CONCEPT(DECL))

#define _XLIB_IGNORE_CONCEPT(DECL) XLIB_PP_CAT(_XLIB_IGNORE_CONCEPT_, DECL)
#define _XLIB_IGNORE_CONCEPT_concept

#endif  // XLIB_CONCEPTS_SUPPORT_CONCEPT_HPP_
