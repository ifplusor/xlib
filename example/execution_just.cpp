#include <iostream>
#include <utility>

#include "xlib/execution/algorithm/just.hpp"
#include "xlib/execution/algorithm/sync_wait.hpp"
#include "xlib/execution/concept/sender.hpp"

using namespace xlib;

int main() {
  execution::sender auto just_7 = execution::just(7);
  auto r = execution::sync_wait(std::move(just_7));
  std::cout << "r = " << r << std::endl;
  return 0;
}
