#include <functional>
#include <iostream>
#include <utility>

#include "xlib/execution/algorithm/just.hpp"
#include "xlib/execution/algorithm/on.hpp"
#include "xlib/execution/algorithm/transform.hpp"
#include "xlib/execution/concept/sender.hpp"
#include "xlib/execution/context/task_queue.hpp"
#include "xlib/execution/sender/connect.hpp"
#include "xlib/execution/support/as_receiver.hpp"

using namespace xlib;

int main() {
  execution::task_queue task_queue;
  /* execution::sender */ auto just_73 = execution::just(7, 3);
  /* execution::sender */ auto sum = execution::transform(std::move(just_73), [](int a, int b) {
    int r = a + b;
    std::cout << "eval: a + b = " << r << std::endl;
    return r;
  });
  /* execution::sender */ auto on = execution::on(std::move(sum), task_queue.scheduler());
  /* execution::sender */ auto power = execution::transform(std::move(on), [](double a) {
    double r = a * a;
    std::cout << "eval: a * a = " << r << std::endl;
    return r;
  });
  auto operation =
      execution::connect(std::move(power), execution::as_receiver<std::function<void(double)>>{[&task_queue](double r) {
                           std::cout << "r = " << r << std::endl;
                           task_queue.stop();
                         }});
  execution::start(operation);
  task_queue.run_forever();
  return 0;
}
