#ifndef XLIB_PREPROCESSOR_UTILITY_HPP_
#define XLIB_PREPROCESSOR_UTILITY_HPP_


#define XLIB_PP_IS_PAREN(x) XLIB_PP_CHECK(XLIB_PP_IS_PAREN_PROBE x)
#define XLIB_PP_IS_PAREN_PROBE(...) XLIB_PP_PROBE(~)

// XLIB_CXX_VA_OPT
#ifndef XLIB_CXX_VA_OPT
#if __cplusplus > 201703L
#define XLIB_CXX_VA_OPT_(...) XLIB_PP_CHECK(__VA_OPT__(, ) 1)
#define XLIB_CXX_VA_OPT XLIB_CXX_VA_OPT_(~)
#else
#define XLIB_CXX_VA_OPT 0
#endif
#endif  // XLIB_CXX_VA_OPT

// The final XLIB_PP_EXPAND here is to avoid
// https://stackoverflow.com/questions/5134523/msvc-doesnt-expand-va-args-correctly
#define XLIB_PP_COUNT(...)                                                                                           \
  XLIB_PP_EXPAND(XLIB_PP_COUNT_(__VA_ARGS__, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, \
                                32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12,  \
                                11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, ))

#define XLIB_PP_COUNT_(_01, _02, _03, _04, _05, _06, _07, _08, _09, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, \
                       _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, \
                       _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, N, ...)                            \
  N

#define XLIB_PP_IIF(BIT) XLIB_PP_CONCAT_(XLIB_PP_IIF_, BIT)
#define XLIB_PP_IIF_0(TRUE, ...) __VA_ARGS__
#define XLIB_PP_IIF_1(TRUE, ...) TRUE

#define XLIB_PP_LPAREN (
#define XLIB_PP_RPAREN )

#define XLIB_PP_NOT(BIT) XLIB_PP_CONCAT_(XLIB_PP_NOT_, BIT)
#define XLIB_PP_NOT_0 1
#define XLIB_PP_NOT_1 0

#define XLIB_PP_EMPTY()
#define XLIB_PP_COMMA() ,
#define XLIB_PP_LBRACE() {
#define XLIB_PP_RBRACE() }
#define XLIB_PP_COMMA_IIF(X) XLIB_PP_IIF(X)(XLIB_PP_EMPTY, XLIB_PP_COMMA)()

#define XLIB_PP_FOR_EACH(M, ...) XLIB_PP_FOR_EACH_N(XLIB_PP_COUNT(__VA_ARGS__), M, __VA_ARGS__)
#define XLIB_PP_FOR_EACH_N(N, M, ...) XLIB_PP_CONCAT(XLIB_PP_FOR_EACH_, N)(M, __VA_ARGS__)
#define XLIB_PP_FOR_EACH_1(M, _1) M(_1)
#define XLIB_PP_FOR_EACH_2(M, _1, _2) M(_1), M(_2)
#define XLIB_PP_FOR_EACH_3(M, _1, _2, _3) M(_1), M(_2), M(_3)
#define XLIB_PP_FOR_EACH_4(M, _1, _2, _3, _4) M(_1), M(_2), M(_3), M(_4)
#define XLIB_PP_FOR_EACH_5(M, _1, _2, _3, _4, _5) M(_1), M(_2), M(_3), M(_4), M(_5)
#define XLIB_PP_FOR_EACH_6(M, _1, _2, _3, _4, _5, _6) M(_1), M(_2), M(_3), M(_4), M(_5), M(_6)
#define XLIB_PP_FOR_EACH_7(M, _1, _2, _3, _4, _5, _6, _7) M(_1), M(_2), M(_3), M(_4), M(_5), M(_6), M(_7)
#define XLIB_PP_FOR_EACH_8(M, _1, _2, _3, _4, _5, _6, _7, _8) M(_1), M(_2), M(_3), M(_4), M(_5), M(_6), M(_7), M(_8)

#define XLIB_PP_PROBE_EMPTY_PROBE_XLIB_PP_PROBE_EMPTY XLIB_PP_PROBE(~)

#define XLIB_PP_PROBE_EMPTY()
#define XLIB_PP_IS_NOT_EMPTY(...) \
  XLIB_PP_EVAL(XLIB_PP_CHECK, XLIB_PP_CONCAT(XLIB_PP_PROBE_EMPTY_PROBE_, XLIB_PP_PROBE_EMPTY __VA_ARGS__()))

#if defined(_MSC_VER) && !defined(__clang__) && (__cplusplus <= 201703L)
#define XLIB_BOOL(...) ::meta::bool_<__VA_ARGS__>::value
#define XLIB_TRUE_FN !::concepts::detail::instance_<decltype(XLIB_true_fn(::concepts::detail::xNil{}))>

#define XLIB_NOT(...) (!XLIB_BOOL(__VA_ARGS__))
#else
#define XLIB_BOOL(...) __VA_ARGS__
#define XLIB_TRUE_FN XLIB_true_fn(::concepts::detail::xNil{})
#define XLIB_NOT(...) (!(__VA_ARGS__))
#endif

#endif  // XLIB_PREPROCESSOR_UTILITY_HPP_
