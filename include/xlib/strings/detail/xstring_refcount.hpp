#ifndef XLIB_STRINGS_DETAIL_XSTREINGREFCOUNT_HPP_
#define XLIB_STRINGS_DETAIL_XSTREINGREFCOUNT_HPP_

#include <cstdlib>

#include <algorithm>
#include <string>

#include "xlib/memory/auto_release.hpp"
#include "xlib/memory/refcount.hpp"

namespace xlib {

namespace detail {

class string_xstring_refcount : public xlib::refcount {
 public:
  string_xstring_refcount(const std::string& str) : str_(str) {}
  string_xstring_refcount(std::string&& str) : str_(std::move(str)) {}

  ~string_xstring_refcount() = default;

  std::string& str() { return str_; }

 private:
  std::string str_;
};

class cstring_xstring_refcount : public xlib::refcount {
 public:
  cstring_xstring_refcount(const char* cstr, auto_release type) : cstr_(cstr), type_(type) {}

  ~cstring_xstring_refcount() {
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

#endif  // XLIB_STRINGS_DETAIL_XSTREINGREFCOUNT_HPP_
