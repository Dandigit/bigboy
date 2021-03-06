#include <bigboy/Timer.h>

#include <iostream>

bool Timer::update(uint8_t cycles) {
    // Check if the divider register needs to be incremented
    m_divClock += cycles;

    if (m_divClock >= DIVIDER_FREQUENCY) {
        m_divClock -= DIVIDER_FREQUENCY;
        ++m_div;
    }

    // Check if the timer register needs to be incremented
    if (!timerEnabled()) return false;

    m_timaClock += cycles;

    if (m_timaClock >= getTimerFrequency()) {
        m_timaClock -= getTimerFrequency();
        if (m_tima == UINT8_MAX) {
            m_tima = m_tma;
            // Request an interrupt!
            return true;
        } else {
            ++m_tima;
        }
    }

    return false;
}

std::vector<AddressSpace> Timer::addressSpaces() const {
    return {{0xFF04, 0xFF07}};
}

uint8_t Timer::readByte(uint16_t address) const {
    switch (address) {
        case 0xFF04: return m_div;
        case 0xFF05: return m_tima;
        case 0xFF06: return m_tma;
        case 0xFF07: return m_tac;
    }

    std::cerr << "warning: memory device Timer does not support reading the address " << address << '\n';
    return 0xFF;
}

void Timer::writeByte(uint16_t address, uint8_t value) {
    switch (address) {
        case 0xFF04:
            m_div = 0;
            return;
        case 0xFF05:
            m_tima = value;
            return;
        case 0xFF06:
            m_tma = value;
            return;
        case 0xFF07:
            m_tac = value;
            return;
    }

    std::cerr << "warning: memory device Timer does not support writing to the address " << address << '\n';
}

void Timer::reset() {
    m_div = 0;
    m_tima = 0;
    m_tac = 0;
}

uint32_t Timer::getTimerFrequency() const {
    switch (static_cast<FrequencySelect>(m_tac & 0b11u)) {
        case FrequencySelect::HZ_4096:
            return CPU_CLOCK_SPEED / 4096;
        case FrequencySelect::HZ_16384:
            return CPU_CLOCK_SPEED / 16384;
        case FrequencySelect::HZ_65536:
            return CPU_CLOCK_SPEED / 65536;
        case FrequencySelect::HZ_262144:
            return CPU_CLOCK_SPEED / 262144;
    }
}