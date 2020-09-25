#ifndef BIGBOY_APU_H
#define BIGBOY_APU_H

#include <bigboy/MMU/MemoryDevice.h>

class APU : public MemoryDevice {
public:
    std::vector<AddressSpace> addressSpaces() const;
    uint8_t readByte(uint16_t address) const;
    void writeByte(uint16_t address, uint8_t value);

private:
    // FF10: Channel 1 sweep
    // - Bits 6-4: Sweep time
    //   - 000: sweep off - no freq change
    //   - 001: 7.8 ms  (1/128Hz)
    //   - 010: 15.6 ms (2/128Hz)
    //   - 011: 23.4 ms (3/128Hz)
    //   - 100: 31.3 ms (4/128Hz)
    //   - 101: 39.1 ms (5/128Hz)
    //   - 110: 46.9 ms (6/128Hz)
    //   - 111: 54.7 ms (7/128Hz)
    // - Bit 3: Sweep increase/decrease
    //   - 0: Addition (frequency increases)
    //   - 1: Subtraction (frequency decreases)
    // - Bits 2-0: Number of sweep shift (0-7)
    uint8_t m_ch1Sweep;

    // FF11: Channel 1 length and duty
    // - Bit 7-6: Wave pattern duty (read/write)
    //   - 00: 12.5% ( _-------_-------_------- )
    //   - 01: 25%   ( __------__------__------ )
    //   - 10: 50%   ( ____----____----____---- ) (normal)
    //   - 11: 75%   ( ______--______--______-- )
    // - Bit 5-0: Sound length data (write) (0-63)
    uint8_t m_ch1LengthAndDuty;

    // FF12: Channel 1 envelope
    // - Bits 7-4: Initial volume of envelope (0-0F)
    // - Bit 3: Envelope direction (0: decrease, 1: increase)
    // - Bits 2-0: Number of envelope sweep (0-7)
    uint8_t m_ch1Envelope;

    // FF13: Channel 1 frequency (lower 8 bits) (write)
    uint8_t m_ch1FrequencyLow;

    // FF14: Channel 1 frequency (higher 3 bits)
    // - Bit 7: Initial (write) (1: restart sound)
    // - Bit 6: Counter (read/write) (1: stop output when length expires)
    // - Bits 2-0: Frequency higher 3 bits (write)
    uint8_t m_ch1FrequencyHigh;
};

#endif