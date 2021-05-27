#ifndef XLIB_EXECUTION_CONTEXT_TASKQUEUE_HPP_
#define XLIB_EXECUTION_CONTEXT_TASKQUEUE_HPP_

#include <exception>  // std::current_exception
#include <utility>    // std::move

#include "xlib/__config.hpp"
#include "xlib/container/conqueue/unbounded_queue.hpp"
#include "xlib/execution/concept/receiver.hpp"
#include "xlib/execution/support/as_invocable.hpp"
#include "xlib/utility/functional/move_only_function.hpp"

namespace xlib::execution {

namespace detail {

template <typename Conqueue = unbounded_queue<move_only_function<void()>>>
struct __task_queue {
  template <typename Derived>
  class context_base;

  template <typename Context>
  class scheduler_impl;

  template <typename Context>
  class executor_impl;

  template <typename Context, typename Receiver>
  struct operation_impl {
    Receiver receiver;
    Context* context;

    void start() noexcept {
      try {
        context->execute(::xlib::execution::as_invocable<Receiver, executor_impl<Context>>{receiver});
      } catch (...) {
        ::xlib::execution::set_error(std::move(receiver), std::current_exception());
      }
    }
  };

  template <typename Context>
  class sender_impl {
   public:
    template <template <typename...> typename Variant, template <typename...> typename Tuple>
    using value_types = Variant<Tuple<>>;

    template <template <typename...> typename Variant>
    using error_types = Variant<std::exception_ptr>;

    static constexpr bool sends_done = true;

    ~sender_impl() = default;

    sender_impl(const sender_impl&) = default;
    sender_impl& operator=(const sender_impl&) = default;

    sender_impl(sender_impl&&) noexcept = default;
    sender_impl& operator=(sender_impl&&) noexcept = default;

    // see-below require(execution::blocking_t::never_t) const;
    // see-below require(execution::blocking_t::possibly_t) const;
    // see-below require(execution::blocking_t::always_t) const;
    // see-below require(execution::relationship_t::continuation_t) const;
    // see-below require(execution::relationship_t::fork_t) const;
    // see-below require(execution::outstanding_work_t::tracked_t) const;
    // see-below require(execution::outstanding_work_t::untracked_t) const;
    // see-below require(const execution::allocator_t<void>& a) const;
    // template<class ProtoAllocator>
    // see-below require(const execution::allocator_t<ProtoAllocator>& a) const;

    // static constexpr execution::bulk_guarantee_t query(execution::bulk_guarantee_t) const;
    // static constexpr execution::mapping_t query(execution::mapping_t) const;
    // execution::blocking_t query(execution::blocking_t) const;
    // execution::relationship_t query(execution::relationship_t) const;
    // execution::outstanding_work_t query(execution::outstanding_work_t) const;
    // Context& query(execution::context_t /*unused*/) const noexcept { return *context_; }
    // see-below query(execution::allocator_t<void>) const noexcept;
    // template<class ProtoAllocator>
    // see-below query(execution::allocator_t<ProtoAllocator>) const noexcept;

    // bool running_in_this_thread() const noexcept;

    friend bool operator==(const sender_impl& a, const sender_impl& b) noexcept { return a.context_ == b.context_; }
    friend bool operator!=(const sender_impl& a, const sender_impl& b) noexcept { return !operator==(a, b); }

    // clang-format off
    xlib_template (typename Receiver)
    (requires receiver_of<Receiver>)
    auto connect(Receiver&& receiver) const {
      return operation_impl<Context, Receiver>{std::forward<Receiver>(receiver), context_};
    }
    // clang-format on

   private:
    friend class scheduler_impl<Context>;
    sender_impl(Context* pool) noexcept : context_(pool) {}

   private:
    Context* context_;
  };

  template <typename Context>
  class scheduler_impl {
   public:
    using sender_type = sender_impl<Context>;

    ~scheduler_impl() = default;

    scheduler_impl(const scheduler_impl&) = default;
    scheduler_impl& operator=(const scheduler_impl&) = default;

    scheduler_impl(scheduler_impl&&) noexcept = default;
    scheduler_impl& operator=(scheduler_impl&&) noexcept = default;

    // see-below require(const execution::allocator_t<void>& a) const;
    // template<class ProtoAllocator>
    // see-below require(const execution::allocator_t<ProtoAllocator>& a) const;

    // Context& query(execution::context_t /*unused*/) const noexcept { return *context_; }
    // see-below query(execution::allocator_t<void>) const noexcept;
    // template<class ProtoAllocator>
    // see-below query(execution::allocator_t<ProtoAllocator>) const noexcept;

    // bool running_in_this_thread() const noexcept;

    friend bool operator==(const scheduler_impl& a, const scheduler_impl& b) noexcept {
      return a.context_ == b.context_;
    }
    friend bool operator!=(const scheduler_impl& a, const scheduler_impl& b) noexcept { return !operator==(a, b); }

    sender_type schedule() noexcept { return sender_type{context_}; }

   private:
    friend context_base<Context>;
    scheduler_impl(Context* context) noexcept : context_(context) {}

   private:
    Context* context_;
  };

  template <typename Context>
  class executor_impl {
   public:
    // TODO
    template <class Function>
    void execute(Function&& f) const {
      context_->execute(std::forward<Function>(f));
    }

    friend bool operator==(const executor_impl& a, const executor_impl& b) noexcept { return a.context_ == b.context_; }
    friend bool operator!=(const executor_impl& a, const executor_impl& b) noexcept { return !operator==(a, b); }

   private:
    friend class context_base<Context>;
    executor_impl(Context* context) noexcept : context_(context) {}

   private:
    Context* context_;
  };

  template <typename Derived>
  class context_base {
   public:
    using scheduler_type = scheduler_impl<Derived>;
    using executor_type = executor_impl<Derived>;

    context_base() = default;

    // Disable copy
    context_base(const context_base&) = delete;
    context_base& operator=(const context_base&) = delete;

    // Disable move
    context_base(context_base&&) = delete;
    context_base& operator=(context_base&&) = delete;

    // stop accepting incoming work and wait for work to drain
    ~context_base() { stop(); }

    // signal all work to complete
    void stop() { task_queue_.close(); }

    bool run_once() {
      handler_type task;
      auto status = task_queue_.wait_pop(task);
      if (status == queue_op_status::success) {
        try {
          task();
        } catch (...) {
          // do nothing
        }
      } else if (status == queue_op_status::closed) {
        // no new tasks
        return false;
      }
      // TODO(James): do not execute submitted tasks after shutdown
      return true;
    }

    void run_forever() {
      for (;;) {
        if (!run_once()) {
          break;
        }
      }
    }

    scheduler_type scheduler() noexcept { return scheduler_type{static_cast<Derived*>(this)}; }

    executor_type executor() noexcept { return executor_type{static_cast<Derived*>(this)}; }

    bool is_shutdown() { return task_queue_.is_closed(); }

   private:
    using handler_type = typename Conqueue::value_type;

    template <typename Context, typename Receiver>
    friend struct operation_impl;
    friend class executor_impl<Derived>;
    void execute(handler_type task) {
      if (task_queue_.wait_push(std::move(task)) == queue_op_status::closed) {
        throw std::logic_error("executor don't accept new tasks.");
      }
    }

   private:
    Conqueue task_queue_;
  };
};

}  // namespace detail

class task_queue : public detail::__task_queue<>::context_base<task_queue> {};

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_CONTEXT_TASKQUEUE_HPP_
