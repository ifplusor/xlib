#ifndef XLIB_EXECUTION_PROPERTY_BLOCKING_HPP_
#define XLIB_EXECUTION_PROPERTY_BLOCKING_HPP_

#include <future>   // std::future, std::promise
#include <utility>  // std::declval, std:move

#include "xlib/property/enumerator_adapter.hpp"
#include "xlib/property/is_applicable_property.hpp"
#include "xlib/execution/property/behavioral_property.hpp"
#include "xlib/execution/property/blocking_adaptation.hpp"
#include "xlib/types/type_traits/enable_if.hpp"

namespace xlib::execution {

/**
 * @brief Describes what guarantees executors provide about the blocking behavior of their execution functions.
 */
struct blocking_t : detail::__behavioral_propety_impl<blocking_t, 3> {
  using detail::__behavioral_propety_impl<blocking_t, 3>::__behavioral_propety_impl;

  using possibly_t = enumerator<0>;
  using always_t = enumerator<1>;
  using never_t = enumerator<2>;

  static constexpr possibly_t possibly{};
  static constexpr always_t always{};
  static constexpr never_t never{};

 private:
  template <typename Executor>
  class adapter : public enumerator_adapter<adapter, Executor, blocking_t, always_t> {
    template <typename T>
    static auto inner_declval() -> decltype(std::declval<Executor>());

   public:
    using enumerator_adapter<adapter, Executor, blocking_t, always_t>::enumerator_adapter;

    template <typename Function>
    auto execute(Function f) const -> decltype(inner_declval<Function>().execute(std::move(f))) {
      std::promise<void> promise;
      std::future<void> future = promise.get_future();
      this->v_.execute([f = std::move(f), p = std::move(promise)]() mutable { f(); });
      future.wait();
    }

    template <typename Function>
    auto twoway_execute(Function f) const -> decltype(inner_declval<Function>().twoway_execute(std::move(f))) {
      auto future = this->v_.twoway_execute(std::move(f));
      future.wait();
      return future;
    }

    template <typename Function, typename SharedFactory>
    auto bulk_execute(Function f, std::size_t n, SharedFactory sf) const
        -> decltype(inner_declval<Function>().bulk_execute(std::move(f), n, std::move(sf))) {
      std::promise<void> promise;
      std::future<void> future = promise.get_future();
      this->v_.bulk_execute([f = std::move(f), p = std::move(promise)](auto i, auto& s) mutable { f(i, s); }, n,
                            std::move(sf));
      future.wait();
    }

    template <typename Function, typename ResultFactory, typename SharedFactory>
    auto bulk_twoway_execute(Function f, std::size_t n, ResultFactory rf, SharedFactory sf) const
        -> decltype(inner_declval<Function>().bulk_twoway_execute(std::move(f), n, std::move(rf), std::move(sf))) {
      auto future = this->v_.bulk_twoway_execute(std::move(f), n, std::move(rf), std::move(sf));
      future.wait();
      return future;
    }
  };

 public:
  template <typename Executor,
            typename = enable_if_t<blocking_adaptation_t::static_query_v<Executor> == blocking_adaptation_t::allowed>>
  friend adapter<Executor> require(Executor ex, always_t /*unused*/) {
    return adapter<Executor>(std::move(ex));
  }
};

constexpr blocking_t blocking{};

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_PROPERTY_BLOCKING_HPP_
