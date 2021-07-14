#include <chrono>
#include <iostream>
#include <thread>

#include "xlib/latch.hpp"

using namespace xlib;

int main() {
  auto* l = new latch(3);

  std::thread t0([l]() {
    std::cout << "t0 in" << std::endl;
    l->wait();
    std::cout << "t0 out, is_ready=" << l->try_wait() << std::endl;
  });

  std::thread t1([l]() {
    std::cout << "t1 in" << std::endl;
    l->wait_for(std::chrono::seconds(2));
    std::cout << "t1 out, is_ready=" << l->try_wait() << std::endl;
  });

  std::this_thread::sleep_for(std::chrono::seconds(3));

  l->count_down(3);

  t0.join();
  t1.join();

  delete l;

  return 0;
}