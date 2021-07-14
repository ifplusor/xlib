#ifndef XLIB_PREPROCESSOR_FACILITIES_EVAL_HPP_
#define XLIB_PREPROCESSOR_FACILITIES_EVAL_HPP_

/**
 * @details
 * use
 *   `XLIB_PP_EVAL(foo, "bar", baz);`
 * will output:
 *   `foo ("bar", baz);`
 */
#define XLIB_PP_EVAL(X, ...) _XLIB_PP_EVAL(X, (__VA_ARGS__))
#define _XLIB_PP_EVAL(X, ARGS) X ARGS

#define XLIB_PP_EVAL2(X, ...) _XLIB_PP_EVAL2(X, (__VA_ARGS__))
#define _XLIB_PP_EVAL2(X, ARGS) X ARGS

#endif  // XLIB_PREPROCESSOR_FACILITIES_EVAL_HPP_
