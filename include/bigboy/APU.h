#ifndef BIGBOY_APU_H
#define BIGBOY_APU_H

#include <array>
#include <bitset>

#include <bigboy/MemoryDevice.h>

class APU : public MemoryDevice {
public:
    void reset();

    void update(uint8_t cycles); // Step n times

    // Collect all samples that have accumulated since we last collected
    std::vector<float> collectSamples();

    std::vector<AddressSpace> addressSpaces() const;
    uint8_t readByte(uint16_t address) const;
    void writeByte(uint16_t address, uint8_t value);

private:
    void step();

    float stepCh1();

    bool isEnabled() const { return (m_enable >> 7u) & 1u; }

    uint16_t ch1Frequency() const { return ((m_ch1FrequencyHigh & 0b00000111) << 8u) | m_ch1FrequencyLow; }
    uint8_t ch1Duty() const { return m_ch1LengthAndDuty >> 6u; }

    // Control registers
    uint8_t m_terminalControl; // NR50: FF24 0x77
    uint8_t m_terminalChannelSelect; // NR51: FF25 0xf3
    uint8_t m_enable; // NR52: FF26 - Enable the APU and read status of each channel 0xf1

    // Channel 1 registers
    uint8_t m_ch1Sweep; // NR10: 0xFF10
    uint8_t m_ch1LengthAndDuty; // NR11: 0xFF11
    uint8_t m_ch1Envelope; // NR12: 0xFF12
    uint8_t m_ch1FrequencyLow; // NR13: 0xFF13
    uint8_t m_ch1FrequencyHigh; // NR14: 0xFF14

    uint16_t m_ch1FrequencyTimer;
    uint8_t m_ch1DutyPosition;

    uint8_t m_cyclesSinceSample;

    std::vector<float> m_sampleBuffer;

    static inline const std::array<std::bitset<8>, 4> m_wavePatternDuties{
            0b00000001,  // 12.5%
            0b00000011,  // 25%
            0b00001111,  // 50%
            0b11111100}; // 75%
};

#endif