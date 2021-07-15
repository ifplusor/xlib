#ifndef XLIB_UTILITY_FUNCTIONAL_INVOKEOP_HPP_
#define XLIB_UTILITY_FUNCTIONAL_INVOKEOP_HPP_

#include <utility>  // std::forward

#include "xlib/__config.hpp"
#include "xlib/types/type_traits/decay.hpp"
#include "xlib/types/type_traits/enable_if.hpp"
#include "xlib/types/type_traits/is_base_of.hpp"
#include "xlib/types/type_traits/is_function.hpp"
#include "xlib/types/type_traits/is_member_function_pointer.hpp"
#include "xlib/types/type_traits/is_member_object_pointer.hpp"
#include "xlib/types/type_traits/is_member_pointer.hpp"
#include "xlib/types/type_traits/is_reference_wrapper.hpp"

namespace xlib::detail {

#if defined(_XLIB_IF_CONSTEXPR)

template <class T, class Type, class T1, class... Args>
constexpr decltype(auto) INVOKE(Type T::*f, T1&& t1, Args&&... args) {
  if constexpr (is_member_function_pointer_v<decltype(f)>) {
    if constexpr (is_base_of_v<T, decay_t<T1>>) {
      return (std::forward<T1>(t1).*f)(std::forward<Args>(args)...);
    } else if constexpr (is_reference_wrapper_v<decay_t<T1>>) {
      return (t1.get().*f)(std::forward<Args>(args)...);
    } else {
      return ((*std::forward<T1>(t1)).*f)(std::forward<Args>(args)...);
    }
  } else {
    static_assert(is_member_object_pointer_v<decltype(f)>);
    static_assert(sizeof...(args) == 0);
    if constexpr (is_base_of_v<T, decay_t<T1>>) {
      return std::forward<T1>(t1).*f;
    } else if constexpr (is_reference_wrapper_v<decay_t<T1>>) {
      return t1.get().*f;
    } else {
      return (*std::forward<T1>(t1)).*f;
    }
  }
}

template <class F, class... Args>
decltype(auto) INVOKE(F&& f, Args&&... args) {
  return std::forward<F>(f)(std::forward<Args>(args)...);
}

#else  // defined(_XLIB_IF_CONSTEXPR)

template <typename Base, typename T, typename Derived, typename... Args>
auto INVOKE(T Base::*pmf, Derived&& ref, Args&&... args)
    -> enable_if_t<is_function<T>::value && is_base_of<Base, decay_t<Derived>>::value,
                   decltype((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...))> {
  return (std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...);
}

template <typename Base, typename T, typename RefWrap, typename... Args>
auto INVOKE(T Base::*pmf, RefWrap&& ref, Args&&... args)
    -> enable_if_t<is_function<T>::value && is_reference_wrapper<decay_t<RefWrap>>::value,
                   decltype((ref.get().*pmf)(std::forward<Args>(args)...))> {
  return (ref.get().*pmf)(std::forward<Args>(args)...);
}

template <typename Base, typename T, typename Ptr, typename... Args>
auto INVOKE(T Base::*pmf, Ptr&& ptr, Args&&... args)
    -> enable_if_t<is_function<T>::value && !is_reference_wrapper<decay_t<Ptr>>::value &&
                       !is_base_of<Base, decay_t<Ptr>>::value,
                   decltype(((*std::forward<Ptr>(ptr)).*pmf)(std::forward<Args>(args)...))> {
  return ((*std::forward<Ptr>(ptr)).*pmf)(std::forward<Args>(args)...);
}

template <typename Base, typename T, typename Derived>
auto INVOKE(T Base::*pmd, Derived&& ref)
    -> enable_if_t<!is_function<T>::value && is_base_of<Base, decay_t<Derived>>::value,
                   decltype(std::forward<Derived>(ref).*pmd)> {
  return std::forward<Derived>(ref).*pmd;
}

template <typename Base, typename T, typename RefWrap>
auto INVOKE(T Base::*pmd, RefWrap&& ref)
    -> enable_if_t<!is_function<T>::value && is_reference_wrapper<decay_t<RefWrap>>::value, decltype(ref.get().*pmd)> {
  return std::forward<RefWrap>(ref.get()).*pmd;
}

template <typename Base, typename T, typename Ptr>
auto INVOKE(T Base::*pmd, Ptr&& ptr)
    -> enable_if_t<!is_function<T>::value && !is_reference_wrapper<decay_t<Ptr>>::value &&
                       !is_base_of<Base, decay_t<Ptr>>::value,
                   decltype((*std::forward<Ptr>(ptr)).*pmd)> {
  return (*std::forward<Ptr>(ptr)).*pmd;
}

template <typename F, typename... Args>
auto INVOKE(F&& f, Args&&... args)
    -> enable_if_t<!is_member_pointer<decay_t<F>>::value, decltype(std::forward<F>(f)(std::forward<Args>(args)...))> {
  return std::forward<F>(f)(std::forward<Args>(args)...);
}

#endif  // defined(_XLIB_IF_CONSTEXPR)

}  // namespace xlib::detail

#endif  // XLIB_UTILITY_FUNCTIONAL_INVOKEOP_HPP_
