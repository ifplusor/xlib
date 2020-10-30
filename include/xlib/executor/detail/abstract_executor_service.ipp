#ifndef XLIB_EXECUTOR_DETAIL_ABSTRACTEXECUTORSERVICE_IPP_
#define XLIB_EXECUTOR_DETAIL_ABSTRACTEXECUTORSERVICE_IPP_

#include "xlib/executor/executor.hpp"

namespace xlib {

class abstract_executor_service : virtual public executor_service {
 public:
  std::future<void> submit(const handler_type& task) override {
    std::unique_ptr<executor_handler> handler(new executor_handler(const_cast<handler_type&>(task)));
    std::future<void> fut = handler->promise_->get_future();
    execute(std::move(handler));
    return fut;
  }
};

}  // namespace xlib

#endif  // XLIB_EXECUTOR_DETAIL_ABSTRACTEXECUTORSERVICE_IPP_
