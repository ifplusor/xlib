#ifndef XLIB_STRING_DETAIL_XSTREINGREFCOUNT_HPP_
#define XLIB_STRING_DETAIL_XSTREINGREFCOUNT_HPP_

#include <cstdlib>

#include <string>
#include <utility>

#include "xlib/memory/auto_release.hpp"
#include "xlib/memory/refcount.hpp"

namespace xlib {

namespace detail {

class shared_string : public xlib::refcount {
 public:
  shared_string(const std::string& str) : str_(str) {}
  shared_string(std::string&& str) : str_(std::move(str)) {}

  ~shared_string() = default;

  std::string& str() { return str_; }

 private:
  std::string str_;
};

class shared_cstring : public xlib::refcount {
 public:
  shared_cstring(const char* cstr, auto_release type) : cstr_(cstr), type_(type) {}

  ~shared_cstring() {
    switch (type_) {
      case auto_release::auto_free:
        std::free(const_cast<char*>(cstr_));
        break;
      case auto_release::auto_delete:
        delete[] cstr_;
        break;
      default:
        break;
    }
  }

 private:
  const char* cstr_;
  auto_release type_;
};

}  // namespace detail

}  // namespace xlib

#endif  // XLIB_STRING_DETAIL_XSTREINGREFCOUNT_HPP_
