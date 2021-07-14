#ifndef XLIB_TYPETRAITS_HPP_
#define XLIB_TYPETRAITS_HPP_

#include <type_traits>

//
// Helper Classes

#include "xlib/types/type_traits/always_false.hpp"
#include "xlib/types/type_traits/integral_constant.hpp"

//
// Primary type categories

#include "xlib/types/type_traits/is_function.hpp"
#include "xlib/types/type_traits/is_integral.hpp"
#include "xlib/types/type_traits/is_member_function_pointer.hpp"
#include "xlib/types/type_traits/is_member_object_pointer.hpp"
#include "xlib/types/type_traits/is_void.hpp"

//
// Composite type categories

#include "xlib/types/type_traits/is_member_pointer.hpp"
#include "xlib/types/type_traits/is_object.hpp"
#include "xlib/types/type_traits/is_reference.hpp"
#include "xlib/types/type_traits/is_reference_wrapper.hpp"

//
// Supported operations

#include "xlib/types/type_traits/is_copy_constructible.hpp"

//
// Type relationships

#include "xlib/types/type_traits/is_base_of.hpp"
#include "xlib/types/type_traits/is_convertible.hpp"
#include "xlib/types/type_traits/is_invocable.hpp"
#include "xlib/types/type_traits/is_same.hpp"

//
// Const-volatility specifiers

#include "xlib/types/type_traits/remove_cv.hpp"

//
// References

#include "xlib/types/type_traits/add_reference.hpp"
#include "xlib/types/type_traits/remove_reference.hpp"

//
// Sign modifiers

#include "xlib/types/type_traits/make_unsigned.hpp"

//
// Miscellaneous transformations

#include "xlib/types/type_traits/common_reference.hpp"
#include "xlib/types/type_traits/conditional.hpp"
#include "xlib/types/type_traits/decay.hpp"
#include "xlib/types/type_traits/enable_if.hpp"
#include "xlib/types/type_traits/invoke_result.hpp"
#include "xlib/types/type_traits/remove_cvref.hpp"
#include "xlib/types/type_traits/void.hpp"

//
// Experimental

#include "xlib/types/type_traits/is_detected.hpp"
#include "xlib/types/type_traits/nonesuch.hpp"

//
// Others

#include "xlib/types/type_traits/xvalue_reference.hpp"

#endif
