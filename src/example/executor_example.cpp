#include <iostream>
#include <random>

#include "executor/executor.hpp"

using namespace xlib;

std::atomic<int> count(0);
std::atomic<int> tmp(0);
std::atomic<int> acc(0);

int main() {
  int consumer_num = 2;
  thread_pool_executor es("Executor", consumer_num);

  std::random_device rd;   // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> dis(0, 100);

  size_t count_pre_thread = 5000000;
  auto producer = [&]() {
    for (int c = 0; c < count_pre_thread; c++) {
      int t = dis(gen);
      acc += t;
      try {
        es.submit([t]() {
          count += t;
          tmp += 1;
        });
      } catch (std::exception& ex) {
        std::cout << "encounter exception." << ex.what() << std::endl;
      }
    }
  };

  size_t producer_num = 3;
  thread_group tg("producer", producer, producer_num);
  tg.start();
  tg.join();

  es.shutdown(false);

  std::cout << "count=" << count << std::endl;
  std::cout << "acc=" << acc << std::endl;

  if (count != acc || tmp != count_pre_thread * producer_num) {
    std::cout << "error!!! tmp:" << tmp << std::endl;
  }

  return 0;
}
