target_sources(
  xlib-headers
  INTERFACE "${XLIB_INCLUDE_DIR}/xlib/concepts/concept.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/concepts/same_as.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/concepts/derived_from.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/concepts/convertible_to.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/concepts/common_reference_with.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/concepts/destructible.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/concepts/constructible_from.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/concepts/move_constructible.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/concepts/copy_constructible.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/concepts/boolean_testable.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/concepts/equality_comparable.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/concepts/invocable.hpp")
