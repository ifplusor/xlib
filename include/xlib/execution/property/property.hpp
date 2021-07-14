#ifndef XLIB_EXECUTION_PROPERTY_HPP_
#define XLIB_EXECUTION_PROPERTY_HPP_

namespace xlib {

namespace execution {

// one-way: fire-and-forgetful execution, return void
// two-way: return a future
enum class directionality { oneway = 1 << 0, twoway = 1 << 1, then = 1 << 2 };

enum class cardinality { single = 1 << 3, bulk = 1 << 4 };

enum class continuations { continuation, not_continuation };

enum class future_task_submissiong { outstanding_work, not_not_outstanding_work };

enum class bulk_forward_progress_guarantees {
  bulk_sequenced_execution,
  bulk_parallel_execution,
  bulk_unsequenced_execution
};

enum class thread_execution_mapping_guarantees { thread_execution_mapping, new_thread_execution_mapping };

enum class allocators { allocator };

}  // namespace execution

}  // namespace xlib

#endif  // XLIB_EXECUTION_PROPERTY_HPP_
