#ifndef XLIB_EXECUTION_SENDER_SUBMITSTATE_HPP_
#define XLIB_EXECUTION_SENDER_SUBMITSTATE_HPP_

#include <exception>

#include "xlib/execution/sender/connect.hpp"
#include "xlib/types/type_traits/remove_cvref.hpp"

namespace xlib::execution {

template <class Sender, class Receiver>
struct submit_state {
  struct submit_receiver {
    submit_state* submit_state_;

    template <class... Arguments>
    requires receiver_of<Receiver, Arguments...>
    void set_value(Arguments&&... arguments) && noexcept(is_nothrow_receiver_of_v<Receiver, Arguments...>) {
      execution::set_value(std::move(submit_state_->receiver_), std::forward<Arguments>(arguments)...);
      delete submit_state_;
    }

    template <class E>
    // requires receiver<R, E>
    void set_error(E&& e) && noexcept {
      execution::set_error(std::move(submit_state_->receiver_), (E &&) e);
      delete submit_state_;
    }

    void set_done() && noexcept {
      execution::set_done(std::move(submit_state_->receiver_));
      delete submit_state_;
    }
  };

  remove_cvref_t<Receiver> receiver_;
  connect_result_t<Sender, submit_receiver> state_;

  submit_state(Sender&& sender, Receiver&& receiver)
      : receiver_(std::forward<Receiver>(receiver)),
        state_(::xlib::execution::connect(std::forward<Sender>(sender), submit_receiver{this})) {}
};

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_SENDER_SUBMITSTATE_HPP_
