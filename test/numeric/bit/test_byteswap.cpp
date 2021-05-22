#include <cstdint>

#include <gtest/gtest.h>

#include "xlib/numeric/bit/bit_cast.hpp"
#include "xlib/numeric/bit/byteswap.hpp"

using namespace xlib;

TEST(Byteswap, UnsignedInteger) {
  uint16_t u16_a{0x1234U};
  uint16_t u16_b{0x3412U};
  ASSERT_EQ(byteswap(u16_a), u16_b);
  ASSERT_EQ(byteswap(u16_b), u16_a);

  uint32_t u32_a{0x12345678U};
  uint32_t u32_b{0x78563412U};
  ASSERT_EQ(byteswap(u32_a), u32_b);
  ASSERT_EQ(byteswap(u32_b), u32_a);

  uint64_t u64_a{0x123456789ABCDEF0ULL};
  uint64_t u64_b{0xF0DEBC9A78563412ULL};
  ASSERT_EQ(byteswap(u64_a), u64_b);
  ASSERT_EQ(byteswap(u64_b), u64_a);
}

TEST(Byteswap, SignedInteger) {
  auto s16_a = bit_cast<int16_t>(static_cast<uint16_t>(0xFEDCU));
  auto s16_b = bit_cast<int16_t>(static_cast<uint16_t>(0xDCFEU));
  ASSERT_EQ(byteswap(s16_a), s16_b);
  ASSERT_EQ(byteswap(s16_b), s16_a);

  auto s32_a = bit_cast<int32_t>(static_cast<uint32_t>(0xFEDCBA98U));
  auto s32_b = bit_cast<int32_t>(static_cast<uint32_t>(0x98BADCFEU));
  ASSERT_EQ(byteswap(s32_a), s32_b);
  ASSERT_EQ(byteswap(s32_b), s32_a);

  auto s64_a = bit_cast<int64_t>(static_cast<uint64_t>(0xFEDCBA9876543210ULL));
  auto s64_b = bit_cast<int64_t>(static_cast<uint64_t>(0x1032547698BADCFEULL));
  ASSERT_EQ(byteswap(s64_a), s64_b);
  ASSERT_EQ(byteswap(s64_b), s64_a);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  testing::GTEST_FLAG(throw_on_failure) = true;
  return RUN_ALL_TESTS();
}
