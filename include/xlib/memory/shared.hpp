#ifndef XLIB_MEMORY_SHARED_HPP_
#define XLIB_MEMORY_SHARED_HPP_

#include <memory.h>

#include "auto_release.hpp"

namespace xlib {

struct shared_object {
  virtual ~shared_object() = default;
};

template <typename T, auto_release release = auto_release::auto_delete>
class _shared_object {
    public:
    shared_object(const T& obj) :

 private:
  T object_;
};

}

#endif
