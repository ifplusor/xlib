#ifndef XLIB_EXECUTION_CONCEPT_HASEXECUTE_HPP_
#define XLIB_EXECUTION_CONCEPT_HASEXECUTE_HPP_

#include <utility>  // std::declval

#include "xlib/concepts/concept.hpp"
#include "xlib/types/type_traits/is_same.hpp"

namespace xlib::execution::detail {

template <typename Executor, typename Function>
xlib_concept __has_member_execute =
    is_same_v<decltype(std::declval<Executor>().execute(std::declval<Function>())), void>;

}  // namespace xlib::execution::detail

#endif  // XLIB_EXECUTION_CONCEPT_HASEXECUTE_HPP_
