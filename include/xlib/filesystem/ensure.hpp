#ifndef XLIB_FILESYSTEM_UTILITY_HPP_
#define XLIB_FILESYSTEM_UTILITY_HPP_

#include <fstream>
#include <system_error>

#include "xlib/filesystem/filesystem.hpp"

namespace xlib::fs {

inline void ensure_directory(const path& directory_path) {
  auto directory_status = status(directory_path);
  if (exists(directory_status)) {
    if (!is_directory(directory_status)) {
      throw filesystem_error("The path exists, but not a directory", directory_path,
                             std::make_error_code(std::errc::file_exists));
    }
  } else {
    create_directories(directory_path);
  }
}

inline void ensure_regular_file(const path& file_path) {
  auto file_status = status(file_path);
  if (exists(file_status)) {
    if (!is_regular_file(file_status)) {
      throw filesystem_error("The path exists, but not a regular file", file_path,
                             std::make_error_code(std::errc::file_exists));
    }
  } else {
    // create file
    std::ofstream ofs(file_path, std::ios_base::binary | std::ios_base::app);
  }
}

inline void ensure_regular_file(const path& file_path, std::size_t file_size) {
  ensure_regular_file(file_path);
  resize_file(file_path, file_size);
}

}  // namespace xlib::fs

#endif  // XLIB_FILESYSTEM_UTILITY_HPP_
