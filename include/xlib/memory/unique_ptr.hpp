#ifndef XLIB_MEMORY_UNIQUEPTR_HPP_
#define XLIB_MEMORY_UNIQUEPTR_HPP_

#include <memory>
#include <utility>  // std::forward

#include "xlib/__config.hpp"

namespace xlib {

using std::unique_ptr;

#if _XLIB_STD_VER >= 2014L

using std::make_unique;

#else

template <class T, class... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return unique_ptr<T>{new T(std::forward<Args>(args)...)};
}

#endif

}  // namespace xlib

#endif  // XLIB_MEMORY_UNIQUEPTR_HPP_
