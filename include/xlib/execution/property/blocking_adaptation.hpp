#ifndef XLIB_EXECUTION_PROPERTY_BLOCKINGADAPTATION_HPP_
#define XLIB_EXECUTION_PROPERTY_BLOCKINGADAPTATION_HPP_

#include "xlib/property/enumeration.hpp"
#include "xlib/property/enumerator_adapter.hpp"
#include "xlib/types/type_traits/enable_if.hpp"

namespace xlib::execution {

struct blocking_adaptation_t : enumeration<blocking_adaptation_t, 2> {
  using enumeration<blocking_adaptation_t, 2>::enumeration;

  using disallowed_t = enumerator<0>;
  using allowed_t = enumerator<1>;

  static constexpr disallowed_t disallowed{};
  static constexpr allowed_t allowed{};

 private:
  template <typename Executor>
  class adapter : public enumerator_adapter<adapter, Executor, blocking_adaptation_t, allowed_t> {
    template <typename T>
    static auto inner_declval() -> decltype(std::declval<Executor>());

   public:
    using enumerator_adapter<adapter, Executor, blocking_adaptation_t, allowed_t>::enumerator_adapter;

    template <typename Function>
    auto execute(Function f) const -> decltype(inner_declval<Function>().execute(std::move(f))) {
      return this->v_.execute(std::move(f));
    }

    template <typename Function>
    auto twoway_execute(Function f) const -> decltype(inner_declval<Function>().twoway_execute(std::move(f))) {
      return this->v_.twoway_execute(std::move(f));
    }

    template <typename Function, typename SharedFactory>
    auto bulk_execute(Function f, std::size_t n, SharedFactory sf) const
        -> decltype(inner_declval<Function>().bulk_execute(std::move(f), n, std::move(sf))) {
      return this->v_.bulk_execute(std::move(f), n, std::move(sf));
    }

    template <typename Function, typename ResultFactory, typename SharedFactory>
    auto bulk_twoway_execute(Function f, std::size_t n, ResultFactory rf, SharedFactory sf) const
        -> decltype(inner_declval<Function>().bulk_twoway_execute(std::move(f), n, std::move(rf), std::move(sf))) {
      return this->v_.bulk_twoway_execute(std::move(f), n, std::move(rf), std::move(sf));
    }
  };

 public:
  template <typename Executor>
  friend adapter<Executor> require(Executor ex, allowed_t /*unused*/) {
    return adapter<Executor>(std::move(ex));
  }
};

constexpr blocking_adaptation_t blocking_adaptation{};

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_PROPERTY_BLOCKINGADAPTATION_HPP_
