#include <string>

#include <gtest/gtest.h>

#include "xlib/utility/format/format.hpp"

using namespace xlib;

TEST(Format, Format) {
  std::string s_a = "Hello, World!";
  std::string s_b = format("{}, {}!", "Hello", "World");
  ASSERT_EQ(s_a, s_b);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  testing::GTEST_FLAG(throw_on_failure) = true;
  return RUN_ALL_TESTS();
}
