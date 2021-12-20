#include <bigboy/APU.h>

#include <iostream>

void APU::reset() {
    m_terminalControl = 0x77;
    m_terminalChannelSelect = 0xF3;
    m_enable = 0xF1;

    m_ch1Sweep = 0x80;
    m_ch1LengthAndDuty = 0xBF;
    m_ch1Envelope = 0xF3;
    m_ch1FrequencyLow = 0xFF;
    m_ch1FrequencyHigh = 0xBF;
}

std::vector<AddressSpace> APU::addressSpaces() const {
    return {
            {0xFF10, 0xFF14}, // Channel 1
            {0xFF16, 0xFF1E}, // Channels 2 & 3
            {0xFF20, 0xFF26}, // Channel 4 & control registers
            {0xFF30, 0xFF3F}}; // Channel 3 wave RAM
}

uint8_t APU::readByte(uint16_t address) const {
    // No registers (except FF26 above, the enable register) are accessible while the APU is disabled
    if (!isEnabled()) {
        std::cerr << "warning: memory device APU does not support reading the address " << address <<  " because it is disabled\n";
        return 0xFF;
    }

    switch (address) {
        case 0xFF24: return m_terminalControl;
        case 0xFF25: return m_terminalChannelSelect;
        case 0xFF26: return m_enable;
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
            break;
        case 0xFF14:
            m_ch1FrequencyHigh = value;
            break;
        default:
            std::cerr << "warning: memory device APU does not support writing to the address " << address << '\n';
    }
}