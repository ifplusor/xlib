#include <iostream>
#include <random>

#include "executor/executor.hpp"

using namespace xlib;

std::atomic<int> count(0);
std::atomic<int> tmp(0);
std::atomic<int> acc(0);

int main() {
  int threads_num = 3;
  thread_pool_executor es(threads_num);

  std::random_device rd;   // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> dis(0, 100);

  std::thread t1([&]() {
    for (int c = 0; c < 5000000; c++) {
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
  });

  std::thread t2([&]() {
    for (int c = 0; c < 5000000; c++) {
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
  });

  t1.join();
  t2.join();

  es.shutdown();

  std::cout << "count=" << count << std::endl;
  std::cout << "acc=" << acc << std::endl;

  if (count != acc || tmp != 10000000) {
    std::cout << "error!!!" << std::endl;
  }

  return 0;
}
