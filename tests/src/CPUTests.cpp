#include "gtest/gtest.h"

#include "../../src/CPU.h"

class CPUTest : public ::testing::Test {
protected:
    CPU cpu{};
};

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
                ASSERT_EQ(cpu.registers().a, static_cast<uint8_t>(aValues[i] + rValues[i])) \
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

        ASSERT_EQ(cpu.registers().a, static_cast<uint8_t>(aValue + aValue));
    }

    #undef TEST_FOR_REGISTER
}