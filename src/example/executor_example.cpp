#include <iostream>
#include "executor/executor.hpp"

int main() {
  int threads_num = 3;
  scheduled_thread_pool_executor es(threads_num);

  auto fut2 = es.submit([]() {
    std::cout << "before sleep 10s" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    std::cout << "after sleep 10s" << std::endl;
  });

  if (threads_num == 1) {
    try {
      fut2.get();
    } catch (const std::exception& ex) {
      std::cout << ex.what() << std::endl;
    }
  }

  //  std::this_thread::sleep_for(std::chrono::seconds(3));

  auto fut = es.schedule(
      []() {
        std::cout << "before sleep 1s" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "after sleep 1s" << std::endl;
      },
      2, seconds);

  es.schedule(
      []() {
        std::cout << "before sleep 5s" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "after sleep 5s" << std::endl;
      },
      1, seconds);

  fut.wait();
  std::cout << "shutdown" << std::endl;
  es.shutdown();

  return 0;
}
