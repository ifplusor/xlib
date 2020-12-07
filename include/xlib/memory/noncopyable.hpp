#ifndef XLIB_MEMORY_NONCOPYABLE_H_
#define XLIB_MEMORY_NONCOPYABLE_H_

namespace xlib {

class noncopyable {
 public:
  ~noncopyable() = default;

 protected:
  noncopyable() = default;
  noncopyable(const noncopyable&) = delete;
  noncopyable& operator=(const noncopyable&) = delete;
};

}  // namespace xlib

#endif  // XLIB_MEMORY_NONCOPYABLE_H_
