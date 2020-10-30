target_sources(
  xlib-headers
  INTERFACE "${prefix}/xlib/executor/detail/abstract_executor_service.hpp"
            "${prefix}/xlib/executor/detail/thread_pool_executor.hpp"
            "${prefix}/xlib/executor/detail/scheduled_thread_pool_executor.hpp")
