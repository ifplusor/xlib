#ifndef XLIB_TYPES_TYPETRAITS_NONESUCH_HPP_
#define XLIB_TYPES_TYPETRAITS_NONESUCH_HPP_

namespace xlib {

struct nonesuch {
  nonesuch() = delete;
  ~nonesuch() = delete;

  nonesuch(const nonesuch&) = delete;
  nonesuch& operator=(const nonesuch&) = delete;

  nonesuch(nonesuch&&) = delete;
  nonesuch& operator=(nonesuch&&) = delete;
};

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_NONESUCH_HPP_
