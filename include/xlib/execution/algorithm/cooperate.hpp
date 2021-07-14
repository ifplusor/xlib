#ifndef XLIB_EXECUTION_ALGORITHM_COOPERATE_HPP_
#define XLIB_EXECUTION_ALGORITHM_COOPERATE_HPP_

#include <exception>  // std::current_exception
#include <memory>     // std::shared_ptr
#include <optional>   // std::optional
#include <tuple>      // std::tuple
#include <utility>    // std::forward, std::move
#include <variant>    // std::variant

#include "xlib/__config.hpp"
#include "xlib/concepts/invocable.hpp"
#include "xlib/execution/concept/receiver.hpp"
#include "xlib/execution/concept/sender.hpp"
#include "xlib/execution/concept/sender_to.hpp"
#include "xlib/execution/receiver/set_done.hpp"
#include "xlib/execution/receiver/set_error.hpp"
#include "xlib/execution/receiver/set_value.hpp"
#include "xlib/execution/sender/connect.hpp"
#include "xlib/execution/type_traits/connect_traits.hpp"
#include "xlib/execution/type_traits/sender_traits.hpp"
#include "xlib/execution/type_traits/sender_value.hpp"
#include "xlib/meta/type_list.hpp"
#include "xlib/meta/type_set.hpp"
#include "xlib/types/type_traits/apply_result.hpp"
#include "xlib/types/type_traits/is_constructible.hpp"
#include "xlib/utility/functional/invoke.hpp"
#include "xlib/utility/scope/scope_exit.hpp"

namespace xlib::execution {

namespace detail {

template <typename ValueProvider, typename ContextProvider, typename Receiver>
struct __cooperate_context_operation_storage;

/**
 * @brief `receiver r2`
 * @details
 * - When `set_value` is called on `r2`, `os2` ’s destructor will be called, the stored value is forwarded to
 *   `output_receiver` on the appropriate choice of `set_value`, `set_error` or `set_done` to match the operation
 *   performed on `r`.
 *
 * - When `set_error` or `set_done` is called on `r2` the parameters propagate to `output_receiver`.
 *
 * @tparam Receiver
 * @tparam Sender
 */
template <typename ValueProvider, typename ContextProvider, typename Receiver>
struct __cooperate_context_receiver {
  Receiver receiver;

  using value_types = typename sender_traits<ValueProvider>::template value_types<type_set, std::tuple>;
  using error_types = typename sender_traits<ValueProvider>::template error_types<type_set>;
  using value_variant_type = typename value_types::template merge<error_types>::list_type::template apply<std::variant>;
  value_variant_type value_variant;

  using context_operation_storage_type =
      __cooperate_context_operation_storage<ValueProvider, ContextProvider, Receiver>;
  std::shared_ptr<context_operation_storage_type> context_operation_storage{
      std::make_shared<context_operation_storage_type>()};

  template <typename... Values>
  void set_value(Values&&... /*unused*/) noexcept {
    using value_type_list = typename value_types::list_type;
    using error_type_list = typename error_types::list_type;

    auto context_operation_gurad =
        make_scope_exit([context_operation_storage = std::move(context_operation_storage)]() mutable {
          // os2’s destructor will be called, and destroy r2.
          context_operation_storage.reset();
        });

    if (set_value_impl<value_type_list::size()>::set_value(receiver, value_variant)) {
      return;
    }
    if (set_error_impl<value_type_list::size(), error_type_list::size()>::set_error(receiver, value_variant)) {
      return;
    }
    ::xlib::execution::set_done(std::move(receiver));
  }

  template <typename Error>
  void set_error(Error&& error) noexcept {
    ::xlib::execution::set_error(std::move(receiver), std::forward<Error>(error));
  }

  void set_done() noexcept { ::xlib::execution::set_done(std::move(receiver)); }

 private:
  template <size_t N, size_t I = 0>
  struct set_value_impl {
    static bool set_value(Receiver& receiver, value_variant_type& value_variant) {
      auto* value = std::get_if<I>(&value_variant);
      if (value != nullptr) {
        std::apply(::xlib::execution::set_value,
                   std::tuple_cat(std::make_tuple<Receiver>(std::move(receiver)), std::move(*value)));
        return true;
      }
      return set_value_impl<N, I + 1>::set_value(receiver, value_variant);
    }
  };
  template <size_t N>
  struct set_value_impl<N, N> {
    static bool set_value(Receiver& receiver, value_variant_type& value_variant) { return false; }
  };

  template <size_t Offset, size_t N, size_t I = 0>
  struct set_error_impl {
    static bool set_error(Receiver& receiver, value_variant_type& value_variant) {
      auto* value = std::get_if<Offset + I>(&value_variant);
      if (value != nullptr) {
        ::xlib::execution::set_error(std::move(receiver), std::move(*value));
        return true;
      }
      return set_error_impl<Offset, N, I + 1>::set_error(receiver, value_variant);
    }
  };
  template <size_t Offset, size_t N>
  struct set_error_impl<Offset, N, N> {
    static bool set_error(Receiver& receiver, value_variant_type& value_variant) { return false; }
  };
};

template <typename ValueProvider, typename ContextProvider, typename Receiver>
struct __cooperate_context_operation_storage {
  using context_operation_type = decltype(::xlib::execution::connect(
      std::declval<ContextProvider>(),
      std::declval<__cooperate_context_receiver<ValueProvider, ContextProvider, Receiver>>()));
  std::optional<context_operation_type> context_operation;
};

/**
 * @brief `receiver r`
 * @details
 * When `set_value`, `set_error` or `set_done` is called on `r`, the parameter is copied and stored as a subobject of a
 * `receiver r2` and `execution::connect(execution::schedule(sch), std::move(r2))` results in an `operation_state os2`
 * which is stored as a subobject of `os`
 *
 * Calls execution::start(os2)
 *
 * @tparam ValueProvider
 * @tparam ContextProvider
 * @tparam Receiver
 */
template <typename ValueProvider, typename ContextProvider, typename Receiver>
struct __cooperate_value_receiver {
  using context_receiver_type = __cooperate_context_receiver<ValueProvider, ContextProvider, Receiver>;

  ContextProvider context_provider;
  Receiver receiver;

  template <typename... Values>
  void set_value(Values&&... values) noexcept {
    std::tuple<Values...> value_tuple{std::forward<Values>(values)...};
    step_next(context_receiver_type{std::move(receiver), std::move(value_tuple)});
  }

  template <typename Error>
  void set_error(Error&& error) noexcept {
    step_next(context_receiver_type{std::move(receiver), std::forward<Error>(error)});
  }

  void set_done() noexcept { step_next(context_receiver_type{std::move(receiver)}); }

 private:
  void step_next(context_receiver_type&& context_receiver) {
    // inject context_operation to context_receiver to keep them alive.
    auto context_operation_storage = context_receiver.context_operation_storage;
    context_operation_storage->context_operation =
        ::xlib::execution::connect(std::move(context_provider), std::move(context_receiver));
    ::xlib::execution::start(*context_operation_storage->context_operation);
  }
};

/**
 * @brief `operation_state os`
 *
 * @tparam ContextProvider
 * @tparam ValueProvider
 * @tparam Receiver
 */
template <typename ValueProvider, typename ContextProvider, typename Receiver>
struct __cooperate_operation {
  using value_operation_type = decltype(::xlib::execution::connect(
      std::declval<ValueProvider>(),
      std::declval<__cooperate_value_receiver<ValueProvider, ContextProvider, Receiver>>()));
  value_operation_type value_operation;

  /**
   * @details When `execution::start` is called on `os`, call `execution::start(ros)`.
   */
  void start() & noexcept { ::xlib::execution::start(value_operation); }
};

/**
 * @brief `sender s2`
 *
 * @tparam ValueProvider
 * @tparam ContextProvider
 */
template <typename ValueProvider, typename ContextProvider>
// FIXME: requires sender<ValueProvider> && sender<ContextProvider>
struct __cooperate_sender {
  ValueProvider value_provider;
  ContextProvider context_provider;

  template <template <typename...> class Variant, template <typename...> class Tuple>
  using value_types = typename sender_traits<ValueProvider>::template value_types<Variant, Tuple>;

  template <template <typename...> class Variant>
  using error_types = typename sender_traits<ContextProvider>::template error_types<type_set>::template merge<
      typename sender_traits<ValueProvider>::template error_types<type_set>,
      type_set<std::exception_ptr>>::list_type::template apply<Variant>;

  static constexpr bool sends_done =
      sender_traits<ContextProvider>::sends_done || sender_traits<ValueProvider>::sends_done;

  /**
   * @details
   * when `connect` is called with some `receiver output_receiver` as `execution::connect(s2, output_receiver)`
   * resulting in an `operation_state os` which is stored as a subobject of the parent `operation_state`
   *
   * - Constructs a `receiver r` and passes `r` to `execution::connect(s, r)` resulting in an `operation_state ros`,
   *   which is stored as a subobject of `os`
   *
   * @tparam Receiver
   * @param receiver output_receiver
   * @return __cooperate_operation<ValueProvider, ContextProvider, Receiver>
   */
  // clang-format off
  xlib_template (typename Receiver)
  (requires receiver<Receiver> /* && sender_to<Host, __parasitize_receiver<Receiver, Parasite>> */)
  auto connect(Receiver&& receiver)
      // noexcept(is_nothrow_connectable_v<Host, __parasitize_receiver<Receiver, Parasite>>)
      -> __cooperate_operation<ValueProvider, ContextProvider, Receiver> {
    __cooperate_value_receiver<ValueProvider, ContextProvider, Receiver> value_reciever{
        std::move(context_provider), std::forward<Receiver>(receiver)};
    auto value_operation = ::xlib::execution::connect(std::move(value_provider), std::move(value_reciever));
    return {std::move(value_operation)};
  }
  // clang-format on
};

/**
 * @brief The name `execution::cooperate` denotes a customization point object.
 */
struct __cooperate_function {
  // clang-format off
  xlib_template (typename ValueProvider, typename ContextProvider)
  (requires sender<ValueProvider> && sender<ContextProvider>)
  auto operator()(ValueProvider&& value_provider, ContextProvider&& context_provider) const noexcept(
      is_nothrow_constructible_v<__cooperate_sender<ValueProvider, ContextProvider>, ValueProvider, ContextProvider>)
      -> __cooperate_sender<ValueProvider, ContextProvider> {
    return {std::forward<ValueProvider>(value_provider), std::forward<ContextProvider>(context_provider)};
  }
  // clang-format on
};

template <typename T = __cooperate_function>
_XLIB_INLINE_VAR constexpr T __cooperate_customization_point{};

}  // namespace detail

inline namespace cpos {

/**
 * @brief
 */
_XLIB_INLINE_VAR constexpr const auto& cooperate = detail::__cooperate_customization_point<>;

}  // namespace cpos

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_ALGORITHM_COOPERATE_HPP_
