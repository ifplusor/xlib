#include <iostream>
#include <typeindex>

#include "xlib/meta/type_list.hpp"

using namespace xlib;

int main() {
  std::cout << typeid(type_list<int, int>).name() << std::endl;
  std::cout << typeid(type_list<int, int, double>::template splice<1, type_list<float, long>>).name() << std::endl;
  std::cout << typeid(type_list<>::template splice<0, type_list<float>>).name() << std::endl;
  return 0;
}
