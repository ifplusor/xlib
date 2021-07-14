#ifndef XLIB_SHAREDMUTEX_HPP_
#define XLIB_SHAREDMUTEX_HPP_

#include "xlib/__config.hpp"

#if _XLIB_STD_VER >= 2014L
#include <shared_mutex>
#endif  // _XLIB_STD_VER >= 2014L

#include "xlib/thread/shared_mutex/shared_lock.hpp"
#include "xlib/thread/shared_mutex/shared_mutex.hpp"

#endif  // XLIB_SHAREDMUTEX_HPP_
