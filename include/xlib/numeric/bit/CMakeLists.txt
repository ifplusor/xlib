target_sources(
  xlib-headers
  INTERFACE "${XLIB_INCLUDE_DIR}/xlib/numeric/bit/bit_cast.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/numeric/bit/endian.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/numeric/bit/byteswap.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/numeric/bit/endian_traits.hpp"
            "${XLIB_INCLUDE_DIR}/xlib/numeric/bit/endian_convert.hpp")
