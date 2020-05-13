#include "gtest/gtest.h"

#include <bigboy/CPU/CPU.h>

class CPUTest : public ::testing::Test {
protected:
    CPU cpu{};
};

TEST_F(CPUTest, registerPairs) {
    constexpr std::array<uint16_t, 5> values{0, 21, 532, 9000, 2};

    #define TEST_REGISTER_PAIR(pair, high, low) \
        do { \
            for (const uint16_t value : values) { \
                cpu.registers().pair = value; \
                EXPECT_EQ(cpu.registers().pair, value); \
                EXPECT_EQ(static_cast<const CPU&>(cpu).registers().pair, value); \
                EXPECT_EQ(cpu.registers().high, (value >> 8u)); \
                EXPECT_EQ(cpu.registers().low, (value & 0xFFu)); \
            } \
        } while (false)

    TEST_REGISTER_PAIR(BC(), b, c);
    TEST_REGISTER_PAIR(DE(), d, e);
    TEST_REGISTER_PAIR(HL(), h, l);

    #undef TEST_REGISTER_PAIR
}

TEST_F(CPUTest, ADDA_r) {
    constexpr std::array<uint8_t, 5> aValues{21, 0, 67, 99, 255};
    constexpr std::array<uint8_t, 5> rValues{10, 2, 255, 0, 33};

    #define TEST_FOR_REGISTER(r, instruction) \
        do { \
            for (size_t i = 0; i < aValues.size(); ++i) { \
                \
                cpu.registers().a = aValues[i]; \
                cpu.registers().r = rValues[i]; \
                \
                cpu.load({static_cast<uint8_t>(OpCode::instruction)}); \
                cpu.exec(); \
                \
                EXPECT_EQ(cpu.registers().a, static_cast<uint8_t>(aValues[i] + rValues[i])) \
                    << "    in test of instruction " << #instruction << " on register " << #r; \
            } \
        } while (false)

    TEST_FOR_REGISTER(b, ADDA_B);
    TEST_FOR_REGISTER(c, ADDA_C);
    TEST_FOR_REGISTER(d, ADDA_D);
    TEST_FOR_REGISTER(e, ADDA_E);
    TEST_FOR_REGISTER(h, ADDA_H);
    TEST_FOR_REGISTER(l, ADDA_L);

    // ADDA_A is a special case
    for (uint8_t aValue : aValues) {
        cpu.registers().a = aValue;

        cpu.load({static_cast<uint8_t>(OpCode::ADDA_A)});
        cpu.exec();

        EXPECT_EQ(cpu.registers().a, static_cast<uint8_t>(aValue + aValue));
    }

    #undef TEST_FOR_REGISTER
}