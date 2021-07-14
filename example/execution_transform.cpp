#include <iostream>
#include <utility>

#include "xlib/execution/algorithm/just.hpp"
#include "xlib/execution/algorithm/sync_wait.hpp"
#include "xlib/execution/algorithm/transform.hpp"
#include "xlib/execution/concept/sender.hpp"

using namespace xlib;

int main() {
  execution::sender auto just_73 = execution::just(7, 3);
  execution::sender auto sum = execution::transform(std::move(just_73), [](int a, int b) { return a + b; });
  execution::sender auto power = execution::transform(std::move(sum), [](double a) { return a * a; });
  auto r = execution::sync_wait(std::move(power));
  std::cout << "r = " << r << std::endl;
  return 0;
}
