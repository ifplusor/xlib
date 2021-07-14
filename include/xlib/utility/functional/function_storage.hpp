#ifndef XLIB_UTILITY_FUNCTIONAL_FUNCTIONSTORAGE_HPP_
#define XLIB_UTILITY_FUNCTIONAL_FUNCTIONSTORAGE_HPP_

#include <memory>   // std::aligned_storage_t
#include <utility>  // std::forward, std::move

#include "xlib/__config.hpp"
#include "xlib/utility/functional/invoke.hpp"
#include "xlib/types/type_traits/conditional.hpp"
#include "xlib/types/type_traits/is_move_constructible.hpp"

namespace xlib::detail {

using __function_buffer = std::aligned_storage_t<sizeof(void*) * 2, alignof(void*)>;

union __function_storage {
  void* generic_pointer{nullptr};
  __function_buffer local_buffer;
};

template <typename T>
struct __function_small_storage_impl {
  template <typename... Args>
  static void create(__function_storage& storage, Args&&... args) {
    new (static_cast<void*>(&storage.local_buffer)) T(std::forward<Args>(args)...);
  }

  static void destroy(__function_storage& storage) noexcept {
    T& value = *static_cast<T*>(static_cast<void*>(&storage.local_buffer));
    value.~T();
  }

  static void move(__function_storage& dst, __function_storage& src) noexcept {
    create(dst, std::move(*static_cast<T*>(static_cast<void*>(&src.local_buffer))));
    destroy(src);
  }

  template <typename R, typename... ArgTypes>
  static R invoke(__function_storage& storage, ArgTypes... args) {
    return ::xlib::invoke(*static_cast<T*>(static_cast<void*>(&storage.local_buffer)), std::forward<ArgTypes>(args)...);
  }
};

template <typename T>
struct __function_large_storage_impl {
  template <typename... Args>
  static void create(__function_storage& storage, Args&&... args) {
    storage.generic_pointer = new T(std::forward<Args>(args)...);
  }

  static void destroy(__function_storage& storage) noexcept { delete static_cast<T*>(storage.generic_pointer); }

  static void move(__function_storage& dst, __function_storage& src) noexcept {
    dst.generic_pointer = src.generic_pointer;
  }

  template <typename R, typename... ArgTypes>
  static R invoke(__function_storage& storage, ArgTypes... args) {
    return ::xlib::invoke(*static_cast<T*>(storage.generic_pointer), std::forward<ArgTypes>(args)...);
  }
};

template <typename T>
struct __function_storage_traits {
  static constexpr bool is_small_object = sizeof(T) <= sizeof(__function_buffer) &&
                                          alignof(__function_buffer) % alignof(T) == 0 &&
                                          is_nothrow_move_constructible_v<T>;

  using function_type =
      conditional_t<is_small_object, __function_small_storage_impl<T>, __function_large_storage_impl<T>>;
};

}  // namespace xlib::detail

#endif  // XLIB_UTILITY_FUNCTIONAL_FUNCTIONSTORAGE_HPP_
