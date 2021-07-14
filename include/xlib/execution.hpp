#ifndef XLIB_EXECUTION_HPP_
#define XLIB_EXECUTION_HPP_

// clang-format off
#include "xlib/execution/receiver/set_value.hpp"
#include "xlib/execution/receiver/set_error.hpp"
#include "xlib/execution/receiver/set_done.hpp"
#include "xlib/execution/executor/execute.hpp"
#include "xlib/execution/sender/connect.hpp"
#include "xlib/execution/operation_state/start.hpp"
#include "xlib/execution/sender/submit.hpp"
#include "xlib/execution/scheduler/schedule.hpp"

#include "xlib/execution/concept/receiver.hpp"
#include "xlib/execution/concept/operation_state.hpp"
#include "xlib/execution/concept/sender.hpp"
#include "xlib/execution/concept/typed_sender.hpp"
#include "xlib/execution/concept/sender_to.hpp"
#include "xlib/execution/concept/scheduler.hpp"
#include "xlib/execution/concept/executor.hpp"

#include "xlib/execution/algorithm/just.hpp"
#include "xlib/execution/algorithm/sync_wait.hpp"
#include "xlib/execution/algorithm/transform.hpp"
// clang-format on

#endif  // XLIB_EXECUTION_HPP_
