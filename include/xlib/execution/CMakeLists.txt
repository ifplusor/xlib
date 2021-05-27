add_subdirectory(concept)

target_sources(
  xlib-headers
  INTERFACE "${XLIB_INCLUDE_DIR}/xlib/execution/set_value.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/execution/set_error.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/execution/set_done.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/execution/execute.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/execution/connect.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/execution/start.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/execution/submit.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/execution/schedule.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/execution/just.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/execution/sync_wait.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/execution/transform.hpp")
