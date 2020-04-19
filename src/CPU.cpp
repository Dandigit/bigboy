#include <iostream>
#include <bitset>
#include "CPU.h"

CPU::Registers::Flags::Flags(uint8_t byte) {
    zero = ((byte >> ZERO_FLAG_BYTE_POSITION) & 0b1) != 0;
    subtract = ((byte >> SUBTRACT_FLAG_BYTE_POSITION) & 0b1) != 0;
    half_carry = ((byte >> HALF_CARRY_FLAG_BYTE_POSITION) & 0b1) != 0;
    carry = ((byte >> CARRY_FLAG_BYTE_POSITION) & 0b1) != 0;
}

CPU::Registers::Flags::operator uint8_t() {
    return (zero ? 1 : 0) << ZERO_FLAG_BYTE_POSITION |
            (subtract ? 1 : 0) << SUBTRACT_FLAG_BYTE_POSITION |
            (half_carry ? 1 : 0) << HALF_CARRY_FLAG_BYTE_POSITION |
            (carry ? 1 : 0) << CARRY_FLAG_BYTE_POSITION;
}

uint16_t CPU::Registers::getAF() {
    return static_cast<uint16_t>(a) << 8
            | static_cast<uint16_t>(f);
}

void CPU::Registers::setAF(uint16_t value) {
    a = static_cast<uint8_t>((value & 0xFF00) >> 8);
    f = static_cast<uint8_t>(value & 0xFF00);
}

uint16_t CPU::Registers::getBC() {
    return static_cast<uint16_t>(b) << 8
           | static_cast<uint16_t>(c);
}

void CPU::Registers::setBC(uint16_t value) {
    b = static_cast<uint8_t>((value & 0xFF00) >> 8);
    c = static_cast<uint8_t>(value & 0xFF00);
}

uint16_t CPU::Registers::getDE() {
    return static_cast<uint16_t>(d) << 8
           | static_cast<uint16_t>(e);
}

void CPU::Registers::setDE(uint16_t value) {
    d = static_cast<uint8_t>((value & 0xFF00) >> 8);
    e = static_cast<uint8_t>(value & 0xFF00);
}

uint16_t CPU::Registers::getHL() {
    return static_cast<uint16_t>(h) << 8
           | static_cast<uint16_t>(l);
}

void CPU::Registers::setHL(uint16_t value) {
    h = static_cast<uint8_t>((value & 0xFF00) >> 8);
    l = static_cast<uint8_t>(value & 0xFF00);
}

CPU::MemoryBus::MemoryBus(std::array<uint8_t, 0xFFFF> memory) :
        m_memory{memory} {}

uint8_t CPU::MemoryBus::readByte(uint16_t address) {
    return m_memory[address];
}

void CPU::MemoryBus::writeByte(uint16_t address, uint8_t value) {
    m_memory[address] = value;
}

void CPU::exec() {
    while (m_pc < 0xFFFF) {
        step();
    }
}

void CPU::step() {
    auto current = static_cast<OpCode>(m_bus.readByte(m_pc++));
    switch (current) {
        case OpCode::ADDA_B:
            ADDA_r(ArithmeticTarget::B);
            break;
        case OpCode::ADDA_C:
            ADDA_r(ArithmeticTarget::C);
            break;
        case OpCode::ADDA_D:
            ADDA_r(ArithmeticTarget::D);
            break;
        case OpCode::ADDA_E:
            ADDA_r(ArithmeticTarget::E);
            break;
        case OpCode::ADDA_H:
            ADDA_r(ArithmeticTarget::H);
            break;
        case OpCode::ADDA_L:
            ADDA_r(ArithmeticTarget::L);
            break;
        case OpCode::ADDA_A:
            ADDA_r(ArithmeticTarget::A);
            break;
        case OpCode::ADDA_n:
            ADDA_n();
            break;
        case OpCode::ADDA_HL:
            ADDA_HL();
            break;
        case OpCode::ADCA_B:
            ADCA_r(ArithmeticTarget::B);
            break;
        case OpCode::ADCA_C:
            ADCA_r(ArithmeticTarget::C);
            break;
        case OpCode::ADCA_D:
            ADCA_r(ArithmeticTarget::D);
            break;
        case OpCode::ADCA_E:
            ADCA_r(ArithmeticTarget::E);
            break;
        case OpCode::ADCA_H:
            ADCA_r(ArithmeticTarget::H);
            break;
        case OpCode::ADCA_L:
            ADCA_r(ArithmeticTarget::L);
            break;
        case OpCode::ADCA_A:
            ADCA_r(ArithmeticTarget::A);
            break;
        case OpCode::ADCA_n:
            ADCA_n();
            break;
        case OpCode::ADCA_HL:
            ADCA_HL();
            break;
        case OpCode::SUB_B:
            SUB_r(ArithmeticTarget::B);
            break;
        case OpCode::SUB_C:
            SUB_r(ArithmeticTarget::C);
            break;
        case OpCode::SUB_D:
            SUB_r(ArithmeticTarget::D);
            break;
        case OpCode::SUB_E:
            SUB_r(ArithmeticTarget::E);
            break;
        case OpCode::SUB_H:
            SUB_r(ArithmeticTarget::H);
            break;
        case OpCode::SUB_L:
            SUB_r(ArithmeticTarget::L);
            break;
        case OpCode::SUB_A:
            SUB_r(ArithmeticTarget::A);
            break;
        case OpCode::SUB_n:
            SUB_n();
            break;
        case OpCode::SUB_HL:
            SUB_HL();
            break;
        case OpCode::SBCA_B:
            SBCA_r(ArithmeticTarget::B);
            break;
        case OpCode::SBCA_C:
            SBCA_r(ArithmeticTarget::C);
            break;
        case OpCode::SBCA_D:
            SBCA_r(ArithmeticTarget::D);
            break;
        case OpCode::SBCA_E:
            SBCA_r(ArithmeticTarget::E);
            break;
        case OpCode::SBCA_H:
            SBCA_r(ArithmeticTarget::H);
            break;
        case OpCode::SBCA_L:
            SBCA_r(ArithmeticTarget::L);
            break;
        case OpCode::SBCA_A:
            SBCA_r(ArithmeticTarget::A);
            break;
        case OpCode::SBCA_n:
            SBCA_n();
            break;
        case OpCode::SBCA_HL:
            SBCA_HL();
            break;
        case OpCode::AND_B:
            AND_r(ArithmeticTarget::B);
            break;
        case OpCode::AND_C:
            AND_r(ArithmeticTarget::C);
            break;
        case OpCode::AND_D:
            AND_r(ArithmeticTarget::D);
            break;
        case OpCode::AND_E:
            AND_r(ArithmeticTarget::E);
            break;
        case OpCode::AND_H:
            AND_r(ArithmeticTarget::H);
            break;
        case OpCode::AND_L:
            AND_r(ArithmeticTarget::L);
            break;
        case OpCode::AND_A:
            AND_r(ArithmeticTarget::A);
            break;
        case OpCode::AND_n:
            AND_n();
            break;
        case OpCode::AND_HL:
            AND_HL();
            break;
        case OpCode::OR_B:
            OR_r(ArithmeticTarget::B);
            break;
        case OpCode::OR_C:
            OR_r(ArithmeticTarget::C);
            break;
        case OpCode::OR_D:
            OR_r(ArithmeticTarget::D);
            break;
        case OpCode::OR_E:
            OR_r(ArithmeticTarget::E);
            break;
        case OpCode::OR_H:
            OR_r(ArithmeticTarget::H);
            break;
        case OpCode::OR_L:
            OR_r(ArithmeticTarget::L);
            break;
        case OpCode::OR_A:
            OR_r(ArithmeticTarget::A);
            break;
        case OpCode::OR_n:
            OR_n();
            break;
        case OpCode::OR_HL:
            OR_HL();
            break;
        case OpCode::XOR_B:
            XOR_r(ArithmeticTarget::B);
            break;
        case OpCode::XOR_C:
            XOR_r(ArithmeticTarget::C);
            break;
        case OpCode::XOR_D:
            XOR_r(ArithmeticTarget::D);
            break;
        case OpCode::XOR_E:
            XOR_r(ArithmeticTarget::E);
            break;
        case OpCode::XOR_H:
            XOR_r(ArithmeticTarget::H);
            break;
        case OpCode::XOR_L:
            XOR_r(ArithmeticTarget::L);
            break;
        case OpCode::XOR_A:
            XOR_r(ArithmeticTarget::A);
            break;
        case OpCode::XOR_n:
            XOR_n();
            break;
        case OpCode::XOR_HL:
            XOR_HL();
            break;
        case OpCode::CP_B:
            CP_r(ArithmeticTarget::B);
            break;
        case OpCode::CP_C:
            CP_r(ArithmeticTarget::C);
            break;
        case OpCode::CP_D:
            CP_r(ArithmeticTarget::D);
            break;
        case OpCode::CP_E:
            CP_r(ArithmeticTarget::E);
            break;
        case OpCode::CP_H:
            CP_r(ArithmeticTarget::H);
            break;
        case OpCode::CP_L:
            CP_r(ArithmeticTarget::L);
            break;
        case OpCode::CP_A:
            CP_r(ArithmeticTarget::A);
            break;
        case OpCode::CP_n:
            CP_n();
            break;
        case OpCode::CP_HL:
            CP_HL();
            break;
        case OpCode::INC_B:
            INC_r(ArithmeticTarget::B);
            break;
        case OpCode::INC_C:
            INC_r(ArithmeticTarget::C);
            break;
        case OpCode::INC_D:
            INC_r(ArithmeticTarget::D);
            break;
        case OpCode::INC_E:
            INC_r(ArithmeticTarget::E);
            break;
        case OpCode::INC_H:
            INC_r(ArithmeticTarget::H);
            break;
        case OpCode::INC_L:
            INC_r(ArithmeticTarget::L);
            break;
        case OpCode::INC_A:
            INC_r(ArithmeticTarget::A);
            break;
        case OpCode::INC_HL:
            INC_HL();
            break;
        case OpCode::DEC_B:
            DEC_r(ArithmeticTarget::B);
            break;
        case OpCode::DEC_C:
            DEC_r(ArithmeticTarget::C);
            break;
        case OpCode::DEC_D:
            DEC_r(ArithmeticTarget::D);
            break;
        case OpCode::DEC_E:
            DEC_r(ArithmeticTarget::E);
            break;
        case OpCode::DEC_H:
            DEC_r(ArithmeticTarget::H);
            break;
        case OpCode::DEC_L:
            DEC_r(ArithmeticTarget::L);
            break;
        case OpCode::DEC_A:
            DEC_r(ArithmeticTarget::A);
            break;
        case OpCode::DEC_HL:
            DEC_HL();
            break;
        case OpCode::DAA:
            DAA();
            break;
        case OpCode::CPL:
            CPL();
            break;
        case OpCode::CCF:
            CCF();
            break;
        case OpCode::SCF:
            SCF();
            break;
        case OpCode::RLCA:
            RLCA();
            break;
        case OpCode::RLA:
            RLA();
            break;
        case OpCode::RRCA:
            RRCA();
            break;
        case OpCode::RRA:
            RRA();
            break;
        case OpCode::PREFIX:
            stepPrefix();
            break;
        default:
            std::cerr << "Unknown instructon: " << std::bitset<8>{static_cast<uint8_t>(current)} << ".\nTerminating bigboy...\n";
            exit(1);
    }
}

void CPU::stepPrefix() {
    auto current = static_cast<PrefixOpCode>(m_pc++);
    switch (current) {
        case PrefixOpCode::RLC_B:
            RLC_r(ArithmeticTarget::B);
            break;
        case PrefixOpCode::RLC_C:
            RLC_r(ArithmeticTarget::C);
            break;
        case PrefixOpCode::RLC_D:
            RLC_r(ArithmeticTarget::D);
            break;
        case PrefixOpCode::RLC_E:
            RLC_r(ArithmeticTarget::E);
            break;
        case PrefixOpCode::RLC_H:
            RLC_r(ArithmeticTarget::H);
            break;
        case PrefixOpCode::RLC_L:
            RLC_r(ArithmeticTarget::L);
            break;
        case PrefixOpCode::RLC_A:
            RLC_r(ArithmeticTarget::A);
            break;
        case PrefixOpCode::RLC_HL:
            RLC_HL();
            break;
        case PrefixOpCode::RL_B:
            RL_r(ArithmeticTarget::B);
            break;
        case PrefixOpCode::RL_C:
            RL_r(ArithmeticTarget::C);
            break;
        case PrefixOpCode::RL_D:
            RL_r(ArithmeticTarget::D);
            break;
        case PrefixOpCode::RL_E:
            RL_r(ArithmeticTarget::E);
            break;
        case PrefixOpCode::RL_H:
            RL_r(ArithmeticTarget::H);
            break;
        case PrefixOpCode::RL_L:
            RL_r(ArithmeticTarget::L);
            break;
        case PrefixOpCode::RL_A:
            RL_r(ArithmeticTarget::A);
            break;
        case PrefixOpCode::RL_HL:
            RL_HL();
            break;
        case PrefixOpCode::RRC_B:
            RRC_r(ArithmeticTarget::B);
            break;
        case PrefixOpCode::RRC_C:
            RRC_r(ArithmeticTarget::C);
            break;
        case PrefixOpCode::RRC_D:
            RRC_r(ArithmeticTarget::D);
            break;
        case PrefixOpCode::RRC_E:
            RRC_r(ArithmeticTarget::E);
            break;
        case PrefixOpCode::RRC_H:
            RRC_r(ArithmeticTarget::H);
            break;
        case PrefixOpCode::RRC_L:
            RRC_r(ArithmeticTarget::L);
            break;
        case PrefixOpCode::RRC_A:
            RRC_r(ArithmeticTarget::A);
            break;
        case PrefixOpCode::RRC_HL:
            RRC_HL();
            break;
        case PrefixOpCode::RR_B:
            RR_r(ArithmeticTarget::B);
            break;
        case PrefixOpCode::RR_C:
            RR_r(ArithmeticTarget::C);
            break;
        case PrefixOpCode::RR_D:
            RR_r(ArithmeticTarget::D);
            break;
        case PrefixOpCode::RR_E:
            RR_r(ArithmeticTarget::E);
            break;
        case PrefixOpCode::RR_H:
            RR_r(ArithmeticTarget::H);
            break;
        case PrefixOpCode::RR_L:
            RR_r(ArithmeticTarget::L);
            break;
        case PrefixOpCode::RR_A:
            RR_r(ArithmeticTarget::A);
            break;
        case PrefixOpCode::RR_HL:
            RR_HL();
            break;
        case PrefixOpCode::SLA_B:
            SLA_r(ArithmeticTarget::B);
            break;
        case PrefixOpCode::SLA_C:
            SLA_r(ArithmeticTarget::C);
            break;
        case PrefixOpCode::SLA_D:
            SLA_r(ArithmeticTarget::D);
            break;
        case PrefixOpCode::SLA_E:
            SLA_r(ArithmeticTarget::E);
            break;
        case PrefixOpCode::SLA_H:
            SLA_r(ArithmeticTarget::H);
            break;
        case PrefixOpCode::SLA_L:
            SLA_r(ArithmeticTarget::L);
            break;
        case PrefixOpCode::SLA_A:
            SLA_r(ArithmeticTarget::A);
            break;
        case PrefixOpCode::SLA_HL:
            SLA_HL();
            break;
        case PrefixOpCode::SRA_B:
            SRA_r(ArithmeticTarget::B);
            break;
        case PrefixOpCode::SRA_C:
            SRA_r(ArithmeticTarget::C);
            break;
        case PrefixOpCode::SRA_D:
            SRA_r(ArithmeticTarget::D);
            break;
        case PrefixOpCode::SRA_E:
            SRA_r(ArithmeticTarget::E);
            break;
        case PrefixOpCode::SRA_H:
            SRA_r(ArithmeticTarget::H);
            break;
        case PrefixOpCode::SRA_L:
            SRA_r(ArithmeticTarget::L);
            break;
        case PrefixOpCode::SRA_A:
            SRA_r(ArithmeticTarget::A);
            break;
        case PrefixOpCode::SRA_HL:
            SRA_HL();
            break;
        case PrefixOpCode::SRL_B:
            SRL_r(ArithmeticTarget::B);
            break;
        case PrefixOpCode::SRL_C:
            SRL_r(ArithmeticTarget::C);
            break;
        case PrefixOpCode::SRL_D:
            SRL_r(ArithmeticTarget::D);
            break;
        case PrefixOpCode::SRL_E:
            SRL_r(ArithmeticTarget::E);
            break;
        case PrefixOpCode::SRL_H:
            SRL_r(ArithmeticTarget::H);
            break;
        case PrefixOpCode::SRL_L:
            SRL_r(ArithmeticTarget::L);
            break;
        case PrefixOpCode::SRL_A:
            SRL_r(ArithmeticTarget::A);
            break;
        case PrefixOpCode::SRL_HL:
            SRL_HL();
            break;
        default:
            std::cerr << "Unknown prefix instructon: " << std::bitset<8>{static_cast<uint8_t>(current)} << ".\nTerminating bigboy...\n";
            exit(1);
    }
}

// Add `value` to the register A, and set/reset the necessary flags
void CPU::add(uint8_t value) {
    uint8_t result = m_registers.a + value;

    m_registers.f.zero = result == 0;
    m_registers.f.subtract = false;
    m_registers.f.carry = result < value;
    m_registers.f.half_carry = (m_registers.a & 0xF) + (value & 0xF) > 0xF;

    m_registers.a = result;
}

void CPU::ADDA_r(CPU::ArithmeticTarget target) {
    switch (target) {
        case ArithmeticTarget::B: add(m_registers.b); break;
        case ArithmeticTarget::C: add(m_registers.c); break;
        case ArithmeticTarget::D: add(m_registers.d); break;
        case ArithmeticTarget::E: add(m_registers.e); break;
        case ArithmeticTarget::H: add(m_registers.h); break;
        case ArithmeticTarget::L: add(m_registers.l); break;
        case ArithmeticTarget::A: add(m_registers.a); break;
    }
}

void CPU::ADDA_n() {
    add(m_bus.readByte(m_pc++));
}

void CPU::ADDA_HL() {
    add(m_bus.readByte(m_registers.getHL()));
}

// Add `value` plus the carry flag to the register A, and set/reset the necessary flags
void CPU::addWithCarry(uint8_t value) {
    add(value + (m_registers.f.carry ? 1 : 0));
}

void CPU::ADCA_r(CPU::ArithmeticTarget target) {
    switch (target) {
        case ArithmeticTarget::B: addWithCarry(m_registers.b); break;
        case ArithmeticTarget::C: addWithCarry(m_registers.c); break;
        case ArithmeticTarget::D: addWithCarry(m_registers.d); break;
        case ArithmeticTarget::E: addWithCarry(m_registers.e); break;
        case ArithmeticTarget::H: addWithCarry(m_registers.h); break;
        case ArithmeticTarget::L: addWithCarry(m_registers.l); break;
        case ArithmeticTarget::A: addWithCarry(m_registers.a); break;
    }
}

void CPU::ADCA_n() {
    addWithCarry(m_bus.readByte(m_pc++));
}

void CPU::ADCA_HL() {
    addWithCarry(m_bus.readByte(m_registers.getHL()));
}

// Subtract `value` from the register A, set the correct flags,
// and store the result in register A
void CPU::subtract(uint8_t value) {
    uint8_t result = m_registers.a - value;

    m_registers.f.zero = result == 0;
    m_registers.f.subtract = true;
    m_registers.f.carry = result > value;
    m_registers.f.half_carry = (m_registers.a & 0x0F) < (value & 0x0F);

    m_registers.a = result;
}

void CPU::SUB_r(CPU::ArithmeticTarget target) {
    switch (target) {
        case ArithmeticTarget::B: subtract(m_registers.b); break;
        case ArithmeticTarget::C: subtract(m_registers.c); break;
        case ArithmeticTarget::D: subtract(m_registers.d); break;
        case ArithmeticTarget::E: subtract(m_registers.e); break;
        case ArithmeticTarget::H: subtract(m_registers.h); break;
        case ArithmeticTarget::L: subtract(m_registers.l); break;
        case ArithmeticTarget::A: subtract(m_registers.a); break;
    }
}

void CPU::SUB_n() {
    subtract(m_bus.readByte(m_pc++));
}

void CPU::SUB_HL() {
    subtract(m_bus.readByte(m_registers.getHL()));
}

void CPU::subtractWithCarry(uint8_t value) {
    subtract(value + (m_registers.f.carry ? 1 : 0));
}

void CPU::SBCA_r(CPU::ArithmeticTarget target) {
    switch (target) {
        case ArithmeticTarget::B: subtractWithCarry(m_registers.b); break;
        case ArithmeticTarget::C: subtractWithCarry(m_registers.c); break;
        case ArithmeticTarget::D: subtractWithCarry(m_registers.d); break;
        case ArithmeticTarget::E: subtractWithCarry(m_registers.e); break;
        case ArithmeticTarget::H: subtractWithCarry(m_registers.h); break;
        case ArithmeticTarget::L: subtractWithCarry(m_registers.l); break;
        case ArithmeticTarget::A: subtractWithCarry(m_registers.a); break;
    }
}

void CPU::SBCA_n() {
    subtractWithCarry(m_bus.readByte(m_pc++));
}

void CPU::SBCA_HL() {
    subtractWithCarry(m_bus.readByte(m_registers.getHL()));
}

void CPU::bitwiseAnd(uint8_t value) {
    m_registers.a &= value;

    m_registers.f.zero = m_registers.a == 0;
    m_registers.f.subtract = false;
    m_registers.f.carry = false;
    m_registers.f.half_carry = true;
}

void CPU::AND_r(CPU::ArithmeticTarget target) {
    switch (target) {
        case ArithmeticTarget::B: bitwiseAnd(m_registers.b); break;
        case ArithmeticTarget::C: bitwiseAnd(m_registers.c); break;
        case ArithmeticTarget::D: bitwiseAnd(m_registers.d); break;
        case ArithmeticTarget::E: bitwiseAnd(m_registers.e); break;
        case ArithmeticTarget::H: bitwiseAnd(m_registers.h); break;
        case ArithmeticTarget::L: bitwiseAnd(m_registers.l); break;
        case ArithmeticTarget::A: bitwiseAnd(m_registers.a); break;
    }
}

void CPU::AND_n() {
    bitwiseAnd(m_bus.readByte(m_pc++));
}

void CPU::AND_HL() {
    bitwiseAnd(m_bus.readByte(m_registers.getHL()));
}

void CPU::bitwiseOr(uint8_t value) {
    m_registers.a |= value;

    m_registers.f.zero = m_registers.a == 0;
    m_registers.f.subtract = false;
    m_registers.f.carry = false;
    m_registers.f.half_carry = false;
}

void CPU::OR_r(CPU::ArithmeticTarget target) {
    switch (target) {
        case ArithmeticTarget::B: bitwiseOr(m_registers.b); break;
        case ArithmeticTarget::C: bitwiseOr(m_registers.c); break;
        case ArithmeticTarget::D: bitwiseOr(m_registers.d); break;
        case ArithmeticTarget::E: bitwiseOr(m_registers.e); break;
        case ArithmeticTarget::H: bitwiseOr(m_registers.h); break;
        case ArithmeticTarget::L: bitwiseOr(m_registers.l); break;
        case ArithmeticTarget::A: bitwiseOr(m_registers.a); break;
    }
}

void CPU::OR_n() {
    bitwiseOr(m_bus.readByte(m_pc++));
}

void CPU::OR_HL() {
    bitwiseOr(m_bus.readByte(m_registers.getHL()));
}

void CPU::bitwiseXor(uint8_t value) {
    m_registers.a ^= value;

    m_registers.f.zero = m_registers.a == 0;
    m_registers.f.subtract = false;
    m_registers.f.carry = false;
    m_registers.f.half_carry = false;
}

void CPU::XOR_r(CPU::ArithmeticTarget target) {
    switch (target) {
        case ArithmeticTarget::B: bitwiseXor(m_registers.b); break;
        case ArithmeticTarget::C: bitwiseXor(m_registers.c); break;
        case ArithmeticTarget::D: bitwiseXor(m_registers.d); break;
        case ArithmeticTarget::E: bitwiseXor(m_registers.e); break;
        case ArithmeticTarget::H: bitwiseXor(m_registers.h); break;
        case ArithmeticTarget::L: bitwiseXor(m_registers.l); break;
        case ArithmeticTarget::A: bitwiseXor(m_registers.a); break;
    }
}

void CPU::XOR_n() {
    bitwiseXor(m_bus.readByte(m_pc++));
}

void CPU::XOR_HL() {
    bitwiseXor(m_bus.readByte(m_registers.getHL()));
}

// Compares `value` with (subtracts it from) the register A, setting the appropriate
// flags but not storing the result.
void CPU::compare(uint8_t value) {
    uint8_t result = m_registers.a - value;

    m_registers.f.zero = result == 0;
    m_registers.f.subtract = true;
    m_registers.f.carry = result > value;
    m_registers.f.half_carry = (m_registers.a & 0x0F) < (value & 0x0F);
}

void CPU::CP_r(CPU::ArithmeticTarget target) {
    switch (target) {
        case ArithmeticTarget::B: compare(m_registers.b); break;
        case ArithmeticTarget::C: compare(m_registers.c); break;
        case ArithmeticTarget::D: compare(m_registers.d); break;
        case ArithmeticTarget::E: compare(m_registers.e); break;
        case ArithmeticTarget::H: compare(m_registers.h); break;
        case ArithmeticTarget::L: compare(m_registers.l); break;
        case ArithmeticTarget::A: compare(m_registers.a); break;
    }
}

void CPU::CP_n() {
    compare(m_bus.readByte(m_pc++));
}

void CPU::CP_HL() {
    compare(m_bus.readByte(m_registers.getHL()));
}

void CPU::increment(uint8_t &target) {
    uint8_t result = target + 1;

    m_registers.f.zero = result == 0;
    m_registers.f.subtract = false;
    m_registers.f.half_carry = (target & (1 << 2)) != 0 && (result & (1 << 2)) == 0;

    target = result;
}

void CPU::INC_r(CPU::ArithmeticTarget target) {
    switch (target) {
        case ArithmeticTarget::B: increment(m_registers.b); break;
        case ArithmeticTarget::C: increment(m_registers.c); break;
        case ArithmeticTarget::D: increment(m_registers.d); break;
        case ArithmeticTarget::E: increment(m_registers.e); break;
        case ArithmeticTarget::H: increment(m_registers.h); break;
        case ArithmeticTarget::L: increment(m_registers.l); break;
        case ArithmeticTarget::A: increment(m_registers.a); break;
    }
}

void CPU::INC_HL() {
    uint8_t dummy = m_bus.readByte(m_registers.getHL());
    increment(dummy);
    m_bus.writeByte(m_registers.getHL(), dummy);
}

void CPU::decrement(uint8_t& target) {
    uint8_t result = target - 1;

    m_registers.f.zero = result == 0;
    m_registers.f.subtract = true;
    m_registers.f.half_carry = ((result ^ 0x01 ^ target) & 0x10) == 0x10;

    target = result;
}

void CPU::DEC_r(CPU::ArithmeticTarget target) {
    switch (target) {
        case ArithmeticTarget::B: decrement(m_registers.b); break;
        case ArithmeticTarget::C: decrement(m_registers.c); break;
        case ArithmeticTarget::D: decrement(m_registers.d); break;
        case ArithmeticTarget::E: decrement(m_registers.e); break;
        case ArithmeticTarget::H: decrement(m_registers.h); break;
        case ArithmeticTarget::L: decrement(m_registers.l); break;
        case ArithmeticTarget::A: decrement(m_registers.a); break;
    }
}

void CPU::DEC_HL() {
    uint8_t dummy = m_bus.readByte(m_registers.getHL());
    decrement(dummy);
    m_bus.writeByte(m_registers.getHL(), dummy);
}

// Retroactively transform the previous addition or subtraction into a BCD operation
void CPU::DAA() {
    if (m_registers.f.subtract) {
        // After a subtraction, only adjust if a carry and/or half carry occurred
        if (m_registers.f.carry) {
            m_registers.a -= 0x60;
        }

        if (m_registers.f.half_carry) {
            m_registers.a -= 0x6;
        }
    } else {
        // After an addition, only adjust if a carry and/or half carry occurred
        // or if the result is out of bounds
        if (m_registers.f.carry || m_registers.a > 0x99) {
            m_registers.a += 0x60;
            m_registers.f.carry = true;
        }

        if (m_registers.f.half_carry || (m_registers.a & 0x0f) > 0x09) {
            m_registers.a += 0x6;
        }
    }

    // Always update these flags
    m_registers.f.zero = m_registers.a == 0;
    m_registers.f.half_carry = false;
}

// Invert the contents of register A
void CPU::CPL() {
    m_registers.a ^= 0xFF;

    m_registers.f.subtract = true;
    m_registers.f.half_carry = true;
}

// Invert the carry flag
void CPU::CCF() {
    m_registers.f.carry = !m_registers.f.carry;

    m_registers.f.subtract = false;
    m_registers.f.half_carry = false;
}

// Set the carry flag
void CPU::SCF() {
    m_registers.f.carry = true;

    m_registers.f.subtract = false;
    m_registers.f.half_carry = false;
}

// Rotate `target` left 1 bit position, copying the sign bit to the carry flag and bit 0
void CPU::rotateLeft(uint8_t &target) {
    uint8_t signBit = target >> 7u;

    target <<= 1u;
    target ^= signBit;

    m_registers.f.carry = signBit != 0;
    m_registers.f.half_carry = false;
    m_registers.f.subtract = false;
}

void CPU::RLCA() {
    rotateLeft(m_registers.a);
}

void CPU::RLC_r(ArithmeticTarget target) {
    switch (target) {
        case ArithmeticTarget::B: rotateLeft(m_registers.b); break;
        case ArithmeticTarget::C: rotateLeft(m_registers.c); break;
        case ArithmeticTarget::D: rotateLeft(m_registers.d); break;
        case ArithmeticTarget::E: rotateLeft(m_registers.e); break;
        case ArithmeticTarget::H: rotateLeft(m_registers.h); break;
        case ArithmeticTarget::L: rotateLeft(m_registers.l); break;
        case ArithmeticTarget::A: rotateLeft(m_registers.a); break;
    }
}

void CPU::RLC_HL() {
    uint8_t dummy = m_bus.readByte(m_registers.getHL());
    rotateLeft(dummy);
    m_bus.writeByte(m_registers.getHL(), dummy);
}

// Rotate `target` left 1 bit position through the carry flag,
// copying the previous contents of the carry flag to bit 0
void CPU::rotateLeftThroughCarry(uint8_t &target) {
    uint8_t prevCarryFlag = (m_registers.f.carry ? 1 : 0);

    std::bitset<9> value{target};
    value[8] = prevCarryFlag;

    value <<= 1u;
    value[0] = prevCarryFlag;

    target = static_cast<uint8_t>(value.to_ulong());

    m_registers.f.carry = value[8];
    m_registers.f.half_carry = false;
    m_registers.f.subtract = false;
}

void CPU::RLA() {
    rotateLeftThroughCarry(m_registers.a);
}

void CPU::RL_r(ArithmeticTarget target) {
    switch (target) {
        case ArithmeticTarget::B: rotateLeftThroughCarry(m_registers.b); break;
        case ArithmeticTarget::C: rotateLeftThroughCarry(m_registers.c); break;
        case ArithmeticTarget::D: rotateLeftThroughCarry(m_registers.d); break;
        case ArithmeticTarget::E: rotateLeftThroughCarry(m_registers.e); break;
        case ArithmeticTarget::H: rotateLeftThroughCarry(m_registers.h); break;
        case ArithmeticTarget::L: rotateLeftThroughCarry(m_registers.l); break;
        case ArithmeticTarget::A: rotateLeftThroughCarry(m_registers.a); break;
    }
}

void CPU::RL_HL() {
    uint8_t dummy = m_bus.readByte(m_registers.getHL());
    rotateLeftThroughCarry(dummy);
    m_bus.writeByte(m_registers.getHL(), dummy);
}

void CPU::rotateRight(uint8_t &target) {
    uint8_t zerothBit = target & 1u;

    target >>= 1u;
    target ^= (zerothBit << 7u);

    m_registers.f.carry = zerothBit != 0;
    m_registers.f.half_carry = false;
    m_registers.f.subtract = false;
}

void CPU::RRCA() {
    rotateRight(m_registers.a);
}

void CPU::RRC_r(ArithmeticTarget target) {
    switch (target) {
        case ArithmeticTarget::B: rotateRight(m_registers.b); break;
        case ArithmeticTarget::C: rotateRight(m_registers.c); break;
        case ArithmeticTarget::D: rotateRight(m_registers.d); break;
        case ArithmeticTarget::E: rotateRight(m_registers.e); break;
        case ArithmeticTarget::H: rotateRight(m_registers.h); break;
        case ArithmeticTarget::L: rotateRight(m_registers.l); break;
        case ArithmeticTarget::A: rotateRight(m_registers.a); break;
    }
}

void CPU::RRC_HL() {
    uint8_t dummy = m_bus.readByte(m_registers.getHL());
    rotateRight(dummy);
    m_bus.writeByte(m_registers.getHL(), dummy);
}

void CPU::rotateRightThroughCarry(uint8_t &target) {
    uint8_t prevCarryFlag = (m_registers.f.carry ? 1 : 0);

    std::bitset<9> value{target};
    value <<= 1u;
    value[0] = prevCarryFlag;

    value >>= 1u;
    value[8] = prevCarryFlag;

    target = static_cast<uint8_t>((value >> 1).to_ulong());

    m_registers.f.carry = value[0];
    m_registers.f.half_carry = false;
    m_registers.f.subtract = false;
}

void CPU::RRA() {
    rotateRightThroughCarry(m_registers.a);
}

void CPU::RR_r(ArithmeticTarget target) {
    switch (target) {
        case ArithmeticTarget::B: rotateRightThroughCarry(m_registers.b); break;
        case ArithmeticTarget::C: rotateRightThroughCarry(m_registers.c); break;
        case ArithmeticTarget::D: rotateRightThroughCarry(m_registers.d); break;
        case ArithmeticTarget::E: rotateRightThroughCarry(m_registers.e); break;
        case ArithmeticTarget::H: rotateRightThroughCarry(m_registers.h); break;
        case ArithmeticTarget::L: rotateRightThroughCarry(m_registers.l); break;
        case ArithmeticTarget::A: rotateRightThroughCarry(m_registers.a); break;
    }
}

void CPU::RR_HL() {
    uint8_t dummy = m_bus.readByte(m_registers.getHL());
    rotateRightThroughCarry(dummy);
    m_bus.writeByte(m_registers.getHL(), dummy);
}

// Shift `target` to the left by 1 bit position, after copying bit 7 into the carry flag
void CPU::shiftLeft(uint8_t& target) {
    m_registers.f.carry = (target >> 7u) != 0;

    target <<= 1u;

    m_registers.f.zero = target == 0;
    m_registers.f.half_carry = false;
    m_registers.f.subtract = false;
}

void CPU::SLA_r(ArithmeticTarget target) {
    switch (target) {
        case ArithmeticTarget::B: shiftLeft(m_registers.b); break;
        case ArithmeticTarget::C: shiftLeft(m_registers.c); break;
        case ArithmeticTarget::D: shiftLeft(m_registers.d); break;
        case ArithmeticTarget::E: shiftLeft(m_registers.e); break;
        case ArithmeticTarget::H: shiftLeft(m_registers.h); break;
        case ArithmeticTarget::L: shiftLeft(m_registers.l); break;
        case ArithmeticTarget::A: shiftLeft(m_registers.a); break;
    }
}

void CPU::SLA_HL() {
    uint8_t dummy = m_bus.readByte(m_registers.getHL());
    shiftLeft(dummy);
    m_bus.writeByte(m_registers.getHL(), dummy);
}

// Shift the lower 7 bits (0-6) of `target` 1 bit position to the right,
// after copying bit 0 into the carry flag.
void CPU::shiftTailRight(uint8_t& target) {
    m_registers.f.carry = (target & 1u) != 0;

    // Save the 7th bit
    uint8_t bit7 = (target >> 7u);

    // Shift the entire number right
    target >>= 1u;

    // Restore the 7th bit
    target |= (bit7 << 7u);

    m_registers.f.zero = target == 0;
    m_registers.f.half_carry = false;
    m_registers.f.subtract = false;
}

void CPU::SRA_r(ArithmeticTarget target) {
    switch (target) {
        case ArithmeticTarget::B: shiftTailRight(m_registers.b); break;
        case ArithmeticTarget::C: shiftTailRight(m_registers.c); break;
        case ArithmeticTarget::D: shiftTailRight(m_registers.d); break;
        case ArithmeticTarget::E: shiftTailRight(m_registers.e); break;
        case ArithmeticTarget::H: shiftTailRight(m_registers.h); break;
        case ArithmeticTarget::L: shiftTailRight(m_registers.l); break;
        case ArithmeticTarget::A: shiftTailRight(m_registers.a); break;
    }
}

void CPU::SRA_HL() {
    uint8_t dummy = m_bus.readByte(m_registers.getHL());
    shiftTailRight(dummy);
    m_bus.writeByte(m_registers.getHL(), dummy);
}

// Shift `target` to the right by 1 bit position, after copying bit 0 into the carry flag
void CPU::shiftRight(uint8_t& target) {
    m_registers.f.carry = (target & 1u) != 0;

    target >>= 1u;

    m_registers.f.zero = target == 0;
    m_registers.f.half_carry = false;
    m_registers.f.subtract = false;
}

void CPU::SRL_r(ArithmeticTarget target) {
    switch (target) {
        case ArithmeticTarget::B: shiftRight(m_registers.b); break;
        case ArithmeticTarget::C: shiftRight(m_registers.c); break;
        case ArithmeticTarget::D: shiftRight(m_registers.d); break;
        case ArithmeticTarget::E: shiftRight(m_registers.e); break;
        case ArithmeticTarget::H: shiftRight(m_registers.h); break;
        case ArithmeticTarget::L: shiftRight(m_registers.l); break;
        case ArithmeticTarget::A: shiftRight(m_registers.a); break;
    }
}

void CPU::SRL_HL() {
    uint8_t dummy = m_bus.readByte(m_registers.getHL());
    shiftRight(dummy);
    m_bus.writeByte(m_registers.getHL(), dummy);
}