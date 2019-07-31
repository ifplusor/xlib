#include <future>
#include <iostream>
#include <thread>

#include "sync/latch.hpp"

using namespace xlib;

int main() {
  auto* l = new latch(3);

  std::thread t0([&]() {
    std::cout << "t0 in" << std::endl;
    l->wait();
    std::cout << "t0 out, is_ready=" << l->is_ready() << std::endl;
  });

  std::thread t1([&]() {
    std::cout << "t1 in" << std::endl;
    l->wait(2, time_unit::seconds);
    std::cout << "t1 out, is_ready=" << l->is_ready() << std::endl;
  });

  std::this_thread::sleep_for(std::chrono::seconds(3));

  l->count_down(3);
  //  delete l;

  t0.join();
  t1.join();

  //  delete l;

  return 0;
}