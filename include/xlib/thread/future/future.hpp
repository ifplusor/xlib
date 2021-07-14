// #include "xlib/scope.hpp"

// template <typename T>
// class future;
//
// namespace detail {
//
// template <typename T>
// class future_state : public std::enable_shared_from_this<future_state<T>> {
// private:
//  mutable std::mutex mutex_;
//};
//
//}  // namespace detail
//
// template <typename T>
// class future {
// public:
//  future() noexcept = default;
//  future(future&& other) noexcept : state_(std::move(other.state_)) {}
//  future(const future& other) = delete;
//
//  future& operator=(future&& other) noexcept {
//    state_ = std::move(other.state_);
//    return *this;
//  }
//
//  future& operator=(const future& other) = delete;
//
//  T get() {
//    auto invalidate_self = make_scope_exit([this] { state_ = nullptr; });
//
//    if (!valid()) {
//      // TODO: we are encouraged to throw an exception here
//    }
//
//    return state_->get();
//  }
//
//  bool valid() const noexcept { return state_ != nullptr; }
//
//  void wait() const {
//    if (!valid()) {
//      // TODO: throw exception here
//    }
//
//    state_->wait();
//  }
//
//  template <class Rep, class Period>
//  std::future_status wait_for(const std::chrono::duration<Rep, Period>& timeout_duration) const;
//
//  template <class Clock, class Duration>
//  std::future_status wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time) const;
//
// private:
//  future(std::shared_ptr<detail::future_state<T>> state) : state_(std::move(state)) {}
//
// private:
//  std::shared_ptr<detail::future_state<T>> state_;
//};