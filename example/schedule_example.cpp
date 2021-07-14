#include <iostream>
#include <random>
#include <thread>

#include "xlib/execution.hpp"

using namespace xlib;

int main() {
  using namespace std::literals::chrono_literals;

  size_t consumer_num = 2;
  scheduled_thread_pool_executor ses("Executor", consumer_num);

  // record start time
  auto start = std::chrono::steady_clock::now();
  ses.schedule(10s, [start]() {
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "one, diff: " << diff.count() << " s\n";
  });

  // record start time
  start = std::chrono::steady_clock::now();
  ses.schedule(5s, [start]() {
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "two, diff: " << diff.count() << " s\n";
  });

  std::this_thread::sleep_for(15s);

  ses.shutdown();

  return 0;
}
