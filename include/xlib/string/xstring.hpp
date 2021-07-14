#ifndef XLIB_STRING_XSTRING_HPP_
#define XLIB_STRING_XSTRING_HPP_

#include <cstring>

#include <string>
#include <typeindex>
#include <utility>

#include "xlib/string/detail/xstring_refcount.hpp"

namespace xlib {

/**
 * Extended string with memory shared.
 */
class xstring {
 public:
  virtual ~xstring() {
    if (refcount_ != nullptr) {
      refcount_->operator--();
    }
  }

  xstring() : xstring("") {}

  xstring(const std::string& str) {
    auto* refcount = new detail::shared_string(str);
    ptr_ = refcount->str().data();
    length_ = refcount->str().length();
    refcount_ = refcount;
  }
  xstring(std::string&& str) {
    auto* refcount = new detail::shared_string(std::move(str));
    ptr_ = refcount->str().data();
    length_ = refcount->str().length();
    refcount_ = refcount;
  }

  xstring(const char* cstr, auto_release release = auto_release::no_release)
      : xstring(cstr, std::strlen(cstr), release) {}
  xstring(const char* cstr, std::size_t length, auto_release release = auto_release::no_release) : refcount_(nullptr) {
    ptr_ = cstr;
    length_ = length;
    if (release != auto_release::no_release) {
      refcount_ = new detail::shared_cstring(cstr, release);
    }
  }

  xstring(const char* data, std::size_t length, refcount* refcount) : ptr_(data), length_(length), refcount_(refcount) {
    if (refcount != nullptr) {
      refcount_->operator++();
    }
  }

  xstring(const xstring& other) : ptr_(other.ptr_), length_(other.length_), refcount_(other.refcount_) {
    if (refcount_ != nullptr) {
      refcount_->operator++();
    }
  }

  xstring(xstring&& other) : ptr_(other.ptr_), length_(other.length_), refcount_(other.refcount_) {
    other.refcount_ = nullptr;
  }

  void operator=(const xstring& other) {
    ptr_ = other.ptr_;
    length_ = other.length_;
    refcount_ = other.refcount_;
    if (refcount_ != nullptr) {
      refcount_->operator++();
    }
  }

  void operator=(xstring&& other) {
    ptr_ = other.ptr_;
    length_ = other.length_;
    refcount_ = other.refcount_;
    other.refcount_ = nullptr;
  }

  int compare(const xstring& other) const noexcept {
    return CompareImpl(
        length_, other.length_,
        Min(length_, other.length_) == 0 ? 0 : std::memcmp(ptr_, other.ptr_, Min(length_, other.length_)));
  }

  bool operator==(const xstring& other) const noexcept {
    return length_ == other.length_ && (empty() || (std::memcmp(ptr_, other.ptr_, length_) == 0));
  }

  bool operator<(const xstring& other) const noexcept { return compare(other) < 0; }

  const char* data() const noexcept { return ptr_; }

  std::size_t length() const noexcept { return length_; }

  bool empty() const noexcept { return length_ == 0; }

  std::string str() const {
    if (std::type_index(typeid(*refcount_)) == std::type_index(typeid(detail::shared_string))) {
      return static_cast<detail::shared_string*>(refcount_)->str();
    } else {
      return std::string(ptr_, length_);
    }
  }

 private:
  static size_t Min(std::size_t length_a, std::size_t length_b) { return length_a < length_b ? length_a : length_b; }

  static int CompareImpl(std::size_t length_a, std::size_t length_b, int compare_result) {
    return compare_result == 0 ? static_cast<int>(length_a > length_b) - static_cast<int>(length_a < length_b)
                               : (compare_result < 0 ? -1 : 1);
  }

 private:
  const char* ptr_;
  std::size_t length_;
  refcount* refcount_;
};

}  // namespace xlib

#endif  // XLIB_STRING_XSTRING_HPP_
