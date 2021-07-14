#include <functional>
#include <iostream>
#include <random>
#include <utility>

#include "xlib/execution/algorithm/just.hpp"
#include "xlib/execution/algorithm/on.hpp"
#include "xlib/execution/operation_state/start.hpp"
#include "xlib/execution/sender/connect.hpp"
#include "xlib/execution/support/as_receiver.hpp"
#include "xlib/execution/thread_pool/static_thread_pool.hpp"

using namespace xlib;

std::atomic<int> count(0);
std::atomic<int> tmp(0);
std::atomic<int> acc(0);

int main() {
  size_t consumer_num = 4;
  static_thread_pool context("Executor", consumer_num);
  auto scheduler = context.scheduler();

  std::random_device rd;   // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> dis(0, 100);

  size_t count_pre_thread = 5000000;
  auto producer = [&]() {
    for (int c = 0; c < count_pre_thread; c++) {
      int t = dis(gen);
      acc += t;
      try {
        auto operation = execution::connect(execution::on(execution::just(t), scheduler),
                                            execution::as_receiver<std::function<void(int)>>{[](int num) {
                                              count += num;
                                              tmp += 1;
                                            }});
        execution::start(operation);
      } catch (std::exception& ex) {
        std::cout << "encounter exception." << ex.what() << std::endl;
      }
    }
  };

  size_t producer_num = 3;
  thread_group tg("producer", producer, producer_num);
  tg.start();
  tg.join();

  context.stop();
  context.wait();

  std::cout << "count=" << count << std::endl;
  std::cout << "acc=" << acc << std::endl;

  if (count != acc || tmp != count_pre_thread * producer_num) {
    std::cout << "error!!! tmp:" << tmp << std::endl;
  }

  return 0;
}
