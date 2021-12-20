#include <bigboy/APU.h>

#include <iostream>

void APU::reset() {
    m_sampleBuffer.clear();

    m_terminalControl = 0x77;
    m_terminalChannelSelect = 0xF3;
    m_enable = 0xF1;

    m_ch1Sweep = 0x80;
    m_ch1LengthAndDuty = 0xBF;
    m_ch1Envelope = 0xF3;
    m_ch1FrequencyLow = 0xFF;
    m_ch1FrequencyHigh = 0xBF;
    m_ch1FrequencyTimer = (2048 - ch1Frequency()) * 4;

    m_cyclesSinceSample = 0;
}

std::vector<float> APU::collectSamples() {
    std::vector<float> buffer = std::move(m_sampleBuffer);
    m_sampleBuffer.clear();
    return buffer;
}

void APU::update(uint8_t cycles) {
    for (int i = cycles; i > 0; --i) {
        step();
    }
}

void APU::step() {
    float ch1Amplitude = stepCh1();

    if (++m_cyclesSinceSample >= 95) {
        m_sampleBuffer.push_back(ch1Amplitude);
        std::cout << "play " << ch1Amplitude << "\n";
        m_cyclesSinceSample -= 95;
    }
}

float APU::stepCh1() {
    if (--m_ch1FrequencyTimer == 0) {
        m_ch1FrequencyTimer = (2048 - ch1Frequency()) * 4;
        ++m_ch1DutyPosition;
    }

    return m_wavePatternDuties[ch1Duty()][m_ch1DutyPosition] ? 1.0f : 0.0f;
}

std::vector<AddressSpace> APU::addressSpaces() const {
    return {
            {0xFF10, 0xFF14}, // Channel 1
            {0xFF16, 0xFF1E}, // Channels 2 & 3
            {0xFF20, 0xFF26}, // Channel 4 & control registers
            {0xFF30, 0xFF3F}}; // Channel 3 wave RAM
}

uint8_t APU::readByte(uint16_t address) const {
    if (address == 0xFF26) return m_enable;

    // No registers (except FF26 above, the enable register) are accessible while the APU is disabled
    if (!isEnabled()) {
        std::cerr << "warning: memory device APU does not support reading the address " << address <<  " because it is disabled\n";
        return 0xFF;
    }

    switch (address) {
        case 0xFF24: return m_terminalControl;
        case 0xFF25: return m_terminalChannelSelect;
        case 0xFF10: return m_ch1Sweep;
        case 0xFF11: return m_ch1LengthAndDuty & 0b11000000; // Only bits 6 and 7 are readable
        case 0xFF12: return m_ch1Envelope;
        case 0xFF14: return m_ch1FrequencyHigh & 0b01000000; // All bits except 6 are write only
        default:
            std::cerr << "warning: memory device APU does not support reading the address " << address << '\n';
            return 0xFF;
    }
}

void APU::writeByte(uint16_t address, uint8_t value) {
    if (address == 0xFF26) {
        // Only bit 7 (global enable/disable) is writeable
        m_enable &= (value | 0b01111111);
        if (!isEnabled()) {
            reset(); // If 0 is written, all registers are reset
        }
    }

    // No registers (except FF26 above, the enable register) are accessible while the APU is disabled
    if (!isEnabled()) {
        std::cerr << "warning: memory device APU does not support writing to the address " << address <<  " because it is disabled\n";
        return;
    }

    switch (address) {
        case 0xFF24:
            m_terminalControl = value;
            break;
        case 0xFF25:
            m_terminalChannelSelect = value;
            break;
        case 0xFF10:
            m_ch1Sweep = value;
            break;
        case 0xFF11:
            m_ch1LengthAndDuty = value;
            break;
        case 0xFF12:
            m_ch1Envelope = value;
            break;
        case 0xFF13:
            m_ch1FrequencyLow = value;
            // Reset the frequency timer when the frequency changes?
            m_ch1FrequencyTimer = (2048 - ch1Frequency()) * 4;
            break;
        case 0xFF14:
            m_ch1FrequencyHigh = value;
            // Reset the frequency timer when the frequency changes?
            m_ch1FrequencyTimer = (2048 - ch1Frequency()) * 4;
            break;
        default:
            std::cerr << "warning: memory device APU does not support writing to the address " << address << '\n';
    }
}