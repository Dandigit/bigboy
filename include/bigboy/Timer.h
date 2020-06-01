#ifndef BIGBOY_TIMER_H
#define BIGBOY_TIMER_H

#include <bigboy/MMU/MemoryDevice.h>

enum class FrequencySelect {
    HZ_4096 = 0b00,
    HZ_262144 = 0b01,
    HZ_65536 = 0b10,
    HZ_16384 = 0b11,
};

class Timer : public MemoryDevice {
public:
    // Returns true if an interrupt is to be requested
    bool update(uint8_t cycles);

    std::vector<AddressSpace> addressSpaces() const override;
    uint8_t readByte(uint16_t address) const override;
    void writeByte(uint16_t address, uint8_t value) override;

    void reset();

private:
    bool timerEnabled() const { return ((m_tac >> 2u) & 1u) != 0; }
    uint32_t getTimerFrequency() const;

    // How many cycles have passed since DIV/TIMA respectively was last incremented
    uint32_t m_divClock = 0;
    uint32_t m_timaClock = 0;

    // FF04: Divider Register (DIV)
    // Incremented at a rate of 16384Hz.
    // Writing will reset it to 0.
    uint8_t m_div = 0;

    // FF05: Timer Counter (TIMA)
    // Incremented at the rate specified by the TAC register.
    // When this value overflows, it will be reset to the value specified
    // by the TMA register, and an interrupt (50) will be requested.
    uint8_t m_tima = 0;

    // FF06: Timer Modulo (TMA)
    // When the TIMA register overflows, it will be reset to this value.
    uint8_t m_tma = 0;

    // FF07: Timer Control (TAC)
    //  - Bit 2: Timer Stop
    //    - 0: Stop
    //    - 1: Start
    //  - Bits 1-0: Input Clock Select (TIMA rate)
    //    - 00: 4096 Hz
    //    - 01: 262144 Hz
    //    - 10: 65536 Hz
    //    - 11: 16384 Hz
    uint8_t m_tac = 0;

    static constexpr uint32_t CPU_CLOCK_SPEED = 4194304; // Hz
    static constexpr uint32_t DIVIDER_FREQUENCY = CPU_CLOCK_SPEED / 16384;
};

#endif //BIGBOY_TIMER_H
