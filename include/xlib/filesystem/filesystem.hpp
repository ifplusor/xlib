#ifndef XLIB_FILESYSTEM_FILESYSTEM_HPP_
#define XLIB_FILESYSTEM_FILESYSTEM_HPP_

#include "xlib/__config.hpp"

#if defined(_XLIB_LIB_FILESYSTEM)

#include <filesystem>

namespace xlib::fs {

using namespace std::filesystem;

}  // namespace xlib::fs

#else  // defined(_XLIB_LIB_FILESYSTEM)

#include <ghc/filesystem.hpp>

namespace xlib {

using namespace ::ghc::filesystem;

}  // namespace xlib

#endif  // defined(_XLIB_LIB_FILESYSTEM)

#endif  // XLIB_FILESYSTEM_FILESYSTEM_HPP_
