#include <iostream>
#include <ostream>
#include <typeindex>

#include "xlib/meta/type_set.hpp"

using namespace xlib;

int main() {
  std::cout << is_one_of_v<int, double, int, float> << std::endl;
  std::cout << typeid(type_set<int, int>::list_type).name() << std::endl;
  std::cout << typeid(type_set<int, double, int>::list_type).name() << std::endl;
  std::cout << typeid(type_set<int, double, int>::insert<double>::list_type).name() << std::endl;
  std::cout << typeid(type_set<int, double, int>::insert<float>::list_type).name() << std::endl;
  std::cout << typeid(type_set<int, double, int>::merge<type_set<float, double>>::list_type).name() << std::endl;
  return 0;
}
