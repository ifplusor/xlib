#ifndef XLIB_SYSTEM_THREAD_HPP_
#define XLIB_SYSTEM_THREAD_HPP_

#include <functional>
#include <memory>
#include <string>
#include <system_error>
#include <thread>

#if defined(_WIN32)
#include <Windows.h>
#elif defined(__APPLE__) || defined(__linux__)
#include <pthread.h>
#if defined(__APPLE__)
#include <sys/proc_info.h>
#endif
#endif

namespace xlib {

#ifdef _WIN32
// From https://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
// Note: The thread name is only set for the thread if the debugger is attached.

const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push, 8)
typedef struct tagTHREADNAME_INFO {
  DWORD dwType;      // Must be 0x1000.
  LPCSTR szName;     // Pointer to name (in user addr space).
  DWORD dwThreadID;  // Thread ID (-1=caller thread).
  DWORD dwFlags;     // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void setWindowsThreadName(DWORD dwThreadID, const char* threadName) {
  THREADNAME_INFO info;
  info.dwType = 0x1000;
  info.szName = threadName;
  info.dwThreadID = dwThreadID;
  info.dwFlags = 0;
#pragma warning(push)
#pragma warning(disable : 6320 6322)
  __try {
    RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
  } __except (EXCEPTION_EXECUTE_HANDLER) {
  }
#pragma warning(pop)
}
#endif

class thread {
 public:
  thread() {}
  thread(const std::string& name) : name_(name) {}

  template <class Function, class... Args>
  thread(const std::string& name, Function&& f, Args&&... args)
      : name_(name), target_(std::bind(std::forward<Function>(f), std::forward<Args>(args)...)) {}

  template <class Function, class... Args>
  void set_target(Function&& f, Args&&... args) {
    target_ = std::bind(std::forward<Function>(f), std::forward<Args>(args)...);
  }

  void start() { thread_.reset(new std::thread(&thread::run_wrapper, this)); }

  virtual void run() {
    if (target_ != nullptr) {
      target_();
    }
  }

  bool joinable() const noexcept {
    if (thread_ != nullptr) {
      return thread_->joinable();
    }
    return false;
  }

  std::thread::id get_id() const noexcept {
    if (thread_ != nullptr) {
      return thread_->get_id();
    }
    return std::thread::id();
  }

  void join() {
    if (thread_ != nullptr) {
      return thread_->join();
    }
    throw std::system_error(std::make_error_code(std::errc::invalid_argument));
  }

  static void set_thread_name(const std::string& name) {
    thread*& t_this = get_this_thread();
    if (t_this != nullptr) {
      if (t_this->name_ == name) {
        return;
      }
      t_this->name_ = name;
    }

#if defined(_WIN32)
    // Naming should not be expensive compared to thread creation and connection set up, but if
    // testing shows otherwise we should make this depend on DEBUG again.
    setWindowsThreadName(::GetCurrentThreadId(), name.c_str());
#elif defined(__APPLE__)
    // Maximum thread name length on OS X is MAXTHREADNAMESIZE (64 characters). This assumes
    // OS X 10.6 or later.
    pthread_setname_np(name.substr(0, MAXTHREADNAMESIZE - 1).c_str());
#elif defined(__linux__)
    // Maximum thread name length supported on Linux is 16 including the null terminator. Ideally
    // we use short and descriptive thread names that fit: this helps for log readibility as well.
    // Since several components set verbose thread names with a uniqifier at the end, we do a split
    // truncation of "first7bytes.last7bytes".
    if (threadName->size() > 15) {
      std::string shortName = name.substr(0, 7) + '.' + name.substr(threadName->size() - 7);
      pthread_setname_np(pthread_self(), shortName.c_str());
    } else {
      pthread_setname_np(pthread_self(), threadName->c_str());
    }
#endif
  }

  static thread*& get_this_thread() {
    static thread_local thread* t_this_;
    return t_this_;
  }

 private:
  void run_wrapper() {
    get_this_thread() = this;

    if (!name_.empty()) {
      std::string name = name_;
      name_.clear();
      set_thread_name(name);
    }

    run();
  }

 private:
  std::string name_;
  std::unique_ptr<std::thread> thread_;
  std::function<void()> target_;
};

}  // namespace xlib

#endif  // XLIB_SYSTEM_THREAD_HPP_
