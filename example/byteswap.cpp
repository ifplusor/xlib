#include <cstdint>

#include <iostream>

#include "xlib/numeric/bit/byteswap.hpp"
#include "xlib/numeric/bit/transform_endian.hpp"

using namespace xlib;

void test_transform_endian();

int main() {
  std::cout << std::hex;

  uint16_t a = 0x1234;
  std::cout << "byteswap(0x1234) = " << byteswap(a) << std::endl;
  std::cout << "tranform_little_endian(0x1234) = " << transform_little_endian(a) << std::endl;
  std::cout << "tranform_big_endian(0x1234) = " << transform_big_endian(a) << std::endl;

  int16_t b = -0x1234;
  std::cout << "byteswap(-0x1234) = " << byteswap(b) << std::endl;
  std::cout << "tranform_little_endian(-0x1234) = " << transform_little_endian(b) << std::endl;
  std::cout << "tranform_big_endian(-0x1234) = " << transform_big_endian(b) << std::endl;

  test_transform_endian();

  return 0;
}
