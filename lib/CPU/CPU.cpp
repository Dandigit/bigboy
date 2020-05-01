#include <iostream>
#include <bitset>

#include <bigboy/CPU/CPU.h>

CPU::Registers::Flags::Flags(uint8_t byte) {
    zero = ((byte >> ZERO_FLAG_BYTE_POSITION) & 0b1) != 0;
    subtract = ((byte >> SUBTRACT_FLAG_BYTE_POSITION) & 0b1) != 0;
    half_carry = ((byte >> HALF_CARRY_FLAG_BYTE_POSITION) & 0b1) != 0;
    carry = ((byte >> CARRY_FLAG_BYTE_POSITION) & 0b1) != 0;
}

CPU::Registers::Flags::operator uint8_t() const {
    return (zero ? 1 : 0) << ZERO_FLAG_BYTE_POSITION |
            (subtract ? 1 : 0) << SUBTRACT_FLAG_BYTE_POSITION |
            (half_carry ? 1 : 0) << HALF_CARRY_FLAG_BYTE_POSITION |
            (carry ? 1 : 0) << CARRY_FLAG_BYTE_POSITION;
}

uint8_t& CPU::Registers::get(TargetRegister target) {
    switch (target) {
        case TargetRegister::B: return b;
        case TargetRegister::C: return c;
        case TargetRegister::D: return d;
        case TargetRegister::E: return e;
        case TargetRegister::H: return h;
        case TargetRegister::L: return l;
        case TargetRegister::A: return a;
    }
}

uint16_t CPU::Registers::getAF() const {
    return static_cast<uint16_t>(a) << 8
            | static_cast<uint16_t>(f);
}

void CPU::Registers::setAF(uint16_t value) {
    a = static_cast<uint8_t>((value & 0xFF00) >> 8);
    f = static_cast<uint8_t>(value & 0xFF00);
}

uint16_t CPU::Registers::getBC() const {
    return static_cast<uint16_t>(b) << 8
           | static_cast<uint16_t>(c);
}

void CPU::Registers::setBC(uint16_t value) {
    b = static_cast<uint8_t>((value & 0xFF00) >> 8);
    c = static_cast<uint8_t>(value & 0xFF00);
}

uint16_t CPU::Registers::getDE() const {
    return static_cast<uint16_t>(d) << 8
           | static_cast<uint16_t>(e);
}

void CPU::Registers::setDE(uint16_t value) {
    d = static_cast<uint8_t>((value & 0xFF00) >> 8);
    e = static_cast<uint8_t>(value & 0xFF00);
}

uint16_t CPU::Registers::getHL() const {
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

CPU::Registers& CPU::registers() {
    return m_registers;
}

void CPU::load(const std::array<uint8_t, 0xFFFF>& memory) {
    m_bus = MemoryBus{memory};
    m_pc = 0;
}

void CPU::exec() {
    while (m_pc < 0xFFFF) {
        step();
    }
}

void CPU::step() {
    auto current = static_cast<OpCode>(m_bus.readByte(m_pc++));
    switch (current) {
        case OpCode::LD_B_B:
            LD_r_r(TargetRegister::B, TargetRegister::B);
            break;
        case OpCode::LD_B_C:
            LD_r_r(TargetRegister::B, TargetRegister::C);
            break;
        case OpCode::LD_B_D:
            LD_r_r(TargetRegister::B, TargetRegister::D);
            break;
        case OpCode::LD_B_E:
            LD_r_r(TargetRegister::B, TargetRegister::E);
            break;
        case OpCode::LD_B_H:
            LD_r_r(TargetRegister::B, TargetRegister::H);
            break;
        case OpCode::LD_B_L:
            LD_r_r(TargetRegister::B, TargetRegister::L);
            break;
        case OpCode::LD_B_A:
            LD_r_r(TargetRegister::B, TargetRegister::A);
            break;
        case OpCode::LD_C_B:
            LD_r_r(TargetRegister::C, TargetRegister::B);
            break;
        case OpCode::LD_C_C:
            LD_r_r(TargetRegister::C, TargetRegister::C);
            break;
        case OpCode::LD_C_D:
            LD_r_r(TargetRegister::C, TargetRegister::D);
            break;
        case OpCode::LD_C_E:
            LD_r_r(TargetRegister::C, TargetRegister::E);
            break;
        case OpCode::LD_C_H:
            LD_r_r(TargetRegister::C, TargetRegister::H);
            break;
        case OpCode::LD_C_L:
            LD_r_r(TargetRegister::C, TargetRegister::L);
            break;
        case OpCode::LD_C_A:
            LD_r_r(TargetRegister::C, TargetRegister::A);
            break;
        case OpCode::LD_D_B:
            LD_r_r(TargetRegister::D, TargetRegister::B);
            break;
        case OpCode::LD_D_C:
            LD_r_r(TargetRegister::D, TargetRegister::C);
            break;
        case OpCode::LD_D_D:
            LD_r_r(TargetRegister::D, TargetRegister::D);
            break;
        case OpCode::LD_D_E:
            LD_r_r(TargetRegister::D, TargetRegister::E);
            break;
        case OpCode::LD_D_H:
            LD_r_r(TargetRegister::D, TargetRegister::H);
            break;
        case OpCode::LD_D_L:
            LD_r_r(TargetRegister::D, TargetRegister::L);
            break;
        case OpCode::LD_D_A:
            LD_r_r(TargetRegister::D, TargetRegister::A);
            break;
        case OpCode::LD_E_B:
            LD_r_r(TargetRegister::E, TargetRegister::B);
            break;
        case OpCode::LD_E_C:
            LD_r_r(TargetRegister::E, TargetRegister::C);
            break;
        case OpCode::LD_E_D:
            LD_r_r(TargetRegister::E, TargetRegister::D);
            break;
        case OpCode::LD_E_E:
            LD_r_r(TargetRegister::E, TargetRegister::E);
            break;
        case OpCode::LD_E_H:
            LD_r_r(TargetRegister::E, TargetRegister::H);
            break;
        case OpCode::LD_E_L:
            LD_r_r(TargetRegister::E, TargetRegister::L);
            break;
        case OpCode::LD_E_A:
            LD_r_r(TargetRegister::E, TargetRegister::A);
            break;
        case OpCode::LD_H_B:
            LD_r_r(TargetRegister::H, TargetRegister::B);
            break;
        case OpCode::LD_H_C:
            LD_r_r(TargetRegister::H, TargetRegister::C);
            break;
        case OpCode::LD_H_D:
            LD_r_r(TargetRegister::H, TargetRegister::D);
            break;
        case OpCode::LD_H_E:
            LD_r_r(TargetRegister::H, TargetRegister::E);
            break;
        case OpCode::LD_H_H:
            LD_r_r(TargetRegister::H, TargetRegister::H);
            break;
        case OpCode::LD_H_L:
            LD_r_r(TargetRegister::H, TargetRegister::L);
            break;
        case OpCode::LD_H_A:
            LD_r_r(TargetRegister::H, TargetRegister::A);
            break;
        case OpCode::LD_L_B:
            LD_r_r(TargetRegister::L, TargetRegister::B);
            break;
        case OpCode::LD_L_C:
            LD_r_r(TargetRegister::L, TargetRegister::C);
            break;
        case OpCode::LD_L_D:
            LD_r_r(TargetRegister::L, TargetRegister::D);
            break;
        case OpCode::LD_L_E:
            LD_r_r(TargetRegister::L, TargetRegister::E);
            break;
        case OpCode::LD_L_H:
            LD_r_r(TargetRegister::L, TargetRegister::H);
            break;
        case OpCode::LD_L_L:
            LD_r_r(TargetRegister::L, TargetRegister::L);
            break;
        case OpCode::LD_L_A:
            LD_r_r(TargetRegister::L, TargetRegister::A);
            break;
        case OpCode::LD_A_B:
            LD_r_r(TargetRegister::A, TargetRegister::B);
            break;
        case OpCode::LD_A_C:
            LD_r_r(TargetRegister::A, TargetRegister::C);
            break;
        case OpCode::LD_A_D:
            LD_r_r(TargetRegister::A, TargetRegister::D);
            break;
        case OpCode::LD_A_E:
            LD_r_r(TargetRegister::A, TargetRegister::E);
            break;
        case OpCode::LD_A_H:
            LD_r_r(TargetRegister::A, TargetRegister::H);
            break;
        case OpCode::LD_A_L:
            LD_r_r(TargetRegister::A, TargetRegister::L);
            break;
        case OpCode::LD_A_A:
            LD_r_r(TargetRegister::A, TargetRegister::A);
            break;
        case OpCode::ADDA_B:
            ADDA_r(TargetRegister::B);
            break;
        case OpCode::ADDA_C:
            ADDA_r(TargetRegister::C);
            break;
        case OpCode::ADDA_D:
            ADDA_r(TargetRegister::D);
            break;
        case OpCode::ADDA_E:
            ADDA_r(TargetRegister::E);
            break;
        case OpCode::ADDA_H:
            ADDA_r(TargetRegister::H);
            break;
        case OpCode::ADDA_L:
            ADDA_r(TargetRegister::L);
            break;
        case OpCode::ADDA_A:
            ADDA_r(TargetRegister::A);
            break;
        case OpCode::ADDA_n:
            ADDA_n();
            break;
        case OpCode::ADDA_HL:
            ADDA_HL();
            break;
        case OpCode::ADCA_B:
            ADCA_r(TargetRegister::B);
            break;
        case OpCode::ADCA_C:
            ADCA_r(TargetRegister::C);
            break;
        case OpCode::ADCA_D:
            ADCA_r(TargetRegister::D);
            break;
        case OpCode::ADCA_E:
            ADCA_r(TargetRegister::E);
            break;
        case OpCode::ADCA_H:
            ADCA_r(TargetRegister::H);
            break;
        case OpCode::ADCA_L:
            ADCA_r(TargetRegister::L);
            break;
        case OpCode::ADCA_A:
            ADCA_r(TargetRegister::A);
            break;
        case OpCode::ADCA_n:
            ADCA_n();
            break;
        case OpCode::ADCA_HL:
            ADCA_HL();
            break;
        case OpCode::SUB_B:
            SUB_r(TargetRegister::B);
            break;
        case OpCode::SUB_C:
            SUB_r(TargetRegister::C);
            break;
        case OpCode::SUB_D:
            SUB_r(TargetRegister::D);
            break;
        case OpCode::SUB_E:
            SUB_r(TargetRegister::E);
            break;
        case OpCode::SUB_H:
            SUB_r(TargetRegister::H);
            break;
        case OpCode::SUB_L:
            SUB_r(TargetRegister::L);
            break;
        case OpCode::SUB_A:
            SUB_r(TargetRegister::A);
            break;
        case OpCode::SUB_n:
            SUB_n();
            break;
        case OpCode::SUB_HL:
            SUB_HL();
            break;
        case OpCode::SBCA_B:
            SBCA_r(TargetRegister::B);
            break;
        case OpCode::SBCA_C:
            SBCA_r(TargetRegister::C);
            break;
        case OpCode::SBCA_D:
            SBCA_r(TargetRegister::D);
            break;
        case OpCode::SBCA_E:
            SBCA_r(TargetRegister::E);
            break;
        case OpCode::SBCA_H:
            SBCA_r(TargetRegister::H);
            break;
        case OpCode::SBCA_L:
            SBCA_r(TargetRegister::L);
            break;
        case OpCode::SBCA_A:
            SBCA_r(TargetRegister::A);
            break;
        case OpCode::SBCA_n:
            SBCA_n();
            break;
        case OpCode::SBCA_HL:
            SBCA_HL();
            break;
        case OpCode::AND_B:
            AND_r(TargetRegister::B);
            break;
        case OpCode::AND_C:
            AND_r(TargetRegister::C);
            break;
        case OpCode::AND_D:
            AND_r(TargetRegister::D);
            break;
        case OpCode::AND_E:
            AND_r(TargetRegister::E);
            break;
        case OpCode::AND_H:
            AND_r(TargetRegister::H);
            break;
        case OpCode::AND_L:
            AND_r(TargetRegister::L);
            break;
        case OpCode::AND_A:
            AND_r(TargetRegister::A);
            break;
        case OpCode::AND_n:
            AND_n();
            break;
        case OpCode::AND_HL:
            AND_HL();
            break;
        case OpCode::OR_B:
            OR_r(TargetRegister::B);
            break;
        case OpCode::OR_C:
            OR_r(TargetRegister::C);
            break;
        case OpCode::OR_D:
            OR_r(TargetRegister::D);
            break;
        case OpCode::OR_E:
            OR_r(TargetRegister::E);
            break;
        case OpCode::OR_H:
            OR_r(TargetRegister::H);
            break;
        case OpCode::OR_L:
            OR_r(TargetRegister::L);
            break;
        case OpCode::OR_A:
            OR_r(TargetRegister::A);
            break;
        case OpCode::OR_n:
            OR_n();
            break;
        case OpCode::OR_HL:
            OR_HL();
            break;
        case OpCode::XOR_B:
            XOR_r(TargetRegister::B);
            break;
        case OpCode::XOR_C:
            XOR_r(TargetRegister::C);
            break;
        case OpCode::XOR_D:
            XOR_r(TargetRegister::D);
            break;
        case OpCode::XOR_E:
            XOR_r(TargetRegister::E);
            break;
        case OpCode::XOR_H:
            XOR_r(TargetRegister::H);
            break;
        case OpCode::XOR_L:
            XOR_r(TargetRegister::L);
            break;
        case OpCode::XOR_A:
            XOR_r(TargetRegister::A);
            break;
        case OpCode::XOR_n:
            XOR_n();
            break;
        case OpCode::XOR_HL:
            XOR_HL();
            break;
        case OpCode::CP_B:
            CP_r(TargetRegister::B);
            break;
        case OpCode::CP_C:
            CP_r(TargetRegister::C);
            break;
        case OpCode::CP_D:
            CP_r(TargetRegister::D);
            break;
        case OpCode::CP_E:
            CP_r(TargetRegister::E);
            break;
        case OpCode::CP_H:
            CP_r(TargetRegister::H);
            break;
        case OpCode::CP_L:
            CP_r(TargetRegister::L);
            break;
        case OpCode::CP_A:
            CP_r(TargetRegister::A);
            break;
        case OpCode::CP_n:
            CP_n();
            break;
        case OpCode::CP_HL:
            CP_HL();
            break;
        case OpCode::INC_B:
            INC_r(TargetRegister::B);
            break;
        case OpCode::INC_C:
            INC_r(TargetRegister::C);
            break;
        case OpCode::INC_D:
            INC_r(TargetRegister::D);
            break;
        case OpCode::INC_E:
            INC_r(TargetRegister::E);
            break;
        case OpCode::INC_H:
            INC_r(TargetRegister::H);
            break;
        case OpCode::INC_L:
            INC_r(TargetRegister::L);
            break;
        case OpCode::INC_A:
            INC_r(TargetRegister::A);
            break;
        case OpCode::INC_HL:
            INC_HL();
            break;
        case OpCode::DEC_B:
            DEC_r(TargetRegister::B);
            break;
        case OpCode::DEC_C:
            DEC_r(TargetRegister::C);
            break;
        case OpCode::DEC_D:
            DEC_r(TargetRegister::D);
            break;
        case OpCode::DEC_E:
            DEC_r(TargetRegister::E);
            break;
        case OpCode::DEC_H:
            DEC_r(TargetRegister::H);
            break;
        case OpCode::DEC_L:
            DEC_r(TargetRegister::L);
            break;
        case OpCode::DEC_A:
            DEC_r(TargetRegister::A);
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
        case OpCode::NOP:
            NOP();
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
            RLC_r(TargetRegister::B);
            break;
        case PrefixOpCode::RLC_C:
            RLC_r(TargetRegister::C);
            break;
        case PrefixOpCode::RLC_D:
            RLC_r(TargetRegister::D);
            break;
        case PrefixOpCode::RLC_E:
            RLC_r(TargetRegister::E);
            break;
        case PrefixOpCode::RLC_H:
            RLC_r(TargetRegister::H);
            break;
        case PrefixOpCode::RLC_L:
            RLC_r(TargetRegister::L);
            break;
        case PrefixOpCode::RLC_A:
            RLC_r(TargetRegister::A);
            break;
        case PrefixOpCode::RLC_HL:
            RLC_HL();
            break;
        case PrefixOpCode::RL_B:
            RL_r(TargetRegister::B);
            break;
        case PrefixOpCode::RL_C:
            RL_r(TargetRegister::C);
            break;
        case PrefixOpCode::RL_D:
            RL_r(TargetRegister::D);
            break;
        case PrefixOpCode::RL_E:
            RL_r(TargetRegister::E);
            break;
        case PrefixOpCode::RL_H:
            RL_r(TargetRegister::H);
            break;
        case PrefixOpCode::RL_L:
            RL_r(TargetRegister::L);
            break;
        case PrefixOpCode::RL_A:
            RL_r(TargetRegister::A);
            break;
        case PrefixOpCode::RL_HL:
            RL_HL();
            break;
        case PrefixOpCode::RRC_B:
            RRC_r(TargetRegister::B);
            break;
        case PrefixOpCode::RRC_C:
            RRC_r(TargetRegister::C);
            break;
        case PrefixOpCode::RRC_D:
            RRC_r(TargetRegister::D);
            break;
        case PrefixOpCode::RRC_E:
            RRC_r(TargetRegister::E);
            break;
        case PrefixOpCode::RRC_H:
            RRC_r(TargetRegister::H);
            break;
        case PrefixOpCode::RRC_L:
            RRC_r(TargetRegister::L);
            break;
        case PrefixOpCode::RRC_A:
            RRC_r(TargetRegister::A);
            break;
        case PrefixOpCode::RRC_HL:
            RRC_HL();
            break;
        case PrefixOpCode::RR_B:
            RR_r(TargetRegister::B);
            break;
        case PrefixOpCode::RR_C:
            RR_r(TargetRegister::C);
            break;
        case PrefixOpCode::RR_D:
            RR_r(TargetRegister::D);
            break;
        case PrefixOpCode::RR_E:
            RR_r(TargetRegister::E);
            break;
        case PrefixOpCode::RR_H:
            RR_r(TargetRegister::H);
            break;
        case PrefixOpCode::RR_L:
            RR_r(TargetRegister::L);
            break;
        case PrefixOpCode::RR_A:
            RR_r(TargetRegister::A);
            break;
        case PrefixOpCode::RR_HL:
            RR_HL();
            break;
        case PrefixOpCode::SLA_B:
            SLA_r(TargetRegister::B);
            break;
        case PrefixOpCode::SLA_C:
            SLA_r(TargetRegister::C);
            break;
        case PrefixOpCode::SLA_D:
            SLA_r(TargetRegister::D);
            break;
        case PrefixOpCode::SLA_E:
            SLA_r(TargetRegister::E);
            break;
        case PrefixOpCode::SLA_H:
            SLA_r(TargetRegister::H);
            break;
        case PrefixOpCode::SLA_L:
            SLA_r(TargetRegister::L);
            break;
        case PrefixOpCode::SLA_A:
            SLA_r(TargetRegister::A);
            break;
        case PrefixOpCode::SLA_HL:
            SLA_HL();
            break;
        case PrefixOpCode::SRA_B:
            SRA_r(TargetRegister::B);
            break;
        case PrefixOpCode::SRA_C:
            SRA_r(TargetRegister::C);
            break;
        case PrefixOpCode::SRA_D:
            SRA_r(TargetRegister::D);
            break;
        case PrefixOpCode::SRA_E:
            SRA_r(TargetRegister::E);
            break;
        case PrefixOpCode::SRA_H:
            SRA_r(TargetRegister::H);
            break;
        case PrefixOpCode::SRA_L:
            SRA_r(TargetRegister::L);
            break;
        case PrefixOpCode::SRA_A:
            SRA_r(TargetRegister::A);
            break;
        case PrefixOpCode::SRA_HL:
            SRA_HL();
            break;
        case PrefixOpCode::SRL_B:
            SRL_r(TargetRegister::B);
            break;
        case PrefixOpCode::SRL_C:
            SRL_r(TargetRegister::C);
            break;
        case PrefixOpCode::SRL_D:
            SRL_r(TargetRegister::D);
            break;
        case PrefixOpCode::SRL_E:
            SRL_r(TargetRegister::E);
            break;
        case PrefixOpCode::SRL_H:
            SRL_r(TargetRegister::H);
            break;
        case PrefixOpCode::SRL_L:
            SRL_r(TargetRegister::L);
            break;
        case PrefixOpCode::SRL_A:
            SRL_r(TargetRegister::A);
            break;
        case PrefixOpCode::SRL_HL:
            SRL_HL();
            break;
        case PrefixOpCode::BIT_0_B:
            BIT_b_r(TargetBit::BIT0, TargetRegister::B);
            break;
        case PrefixOpCode::BIT_1_B:
            BIT_b_r(TargetBit::BIT1, TargetRegister::B);
            break;
        case PrefixOpCode::BIT_2_B:
            BIT_b_r(TargetBit::BIT2, TargetRegister::B);
            break;
        case PrefixOpCode::BIT_3_B:
            BIT_b_r(TargetBit::BIT3, TargetRegister::B);
            break;
        case PrefixOpCode::BIT_4_B:
            BIT_b_r(TargetBit::BIT4, TargetRegister::B);
            break;
        case PrefixOpCode::BIT_5_B:
            BIT_b_r(TargetBit::BIT5, TargetRegister::B);
            break;
        case PrefixOpCode::BIT_6_B:
            BIT_b_r(TargetBit::BIT6, TargetRegister::B);
            break;
        case PrefixOpCode::BIT_7_B:
            BIT_b_r(TargetBit::BIT7, TargetRegister::B);
            break;
        case PrefixOpCode::BIT_0_C:
            BIT_b_r(TargetBit::BIT0, TargetRegister::C);
            break;
        case PrefixOpCode::BIT_1_C:
            BIT_b_r(TargetBit::BIT1, TargetRegister::C);
            break;
        case PrefixOpCode::BIT_2_C:
            BIT_b_r(TargetBit::BIT2, TargetRegister::C);
            break;
        case PrefixOpCode::BIT_3_C:
            BIT_b_r(TargetBit::BIT3, TargetRegister::C);
            break;
        case PrefixOpCode::BIT_4_C:
            BIT_b_r(TargetBit::BIT4, TargetRegister::C);
            break;
        case PrefixOpCode::BIT_5_C:
            BIT_b_r(TargetBit::BIT5, TargetRegister::C);
            break;
        case PrefixOpCode::BIT_6_C:
            BIT_b_r(TargetBit::BIT6, TargetRegister::C);
            break;
        case PrefixOpCode::BIT_7_C:
            BIT_b_r(TargetBit::BIT7, TargetRegister::C);
            break;
        case PrefixOpCode::BIT_0_D:
            BIT_b_r(TargetBit::BIT0, TargetRegister::D);
            break;
        case PrefixOpCode::BIT_1_D:
            BIT_b_r(TargetBit::BIT1, TargetRegister::D);
            break;
        case PrefixOpCode::BIT_2_D:
            BIT_b_r(TargetBit::BIT2, TargetRegister::D);
            break;
        case PrefixOpCode::BIT_3_D:
            BIT_b_r(TargetBit::BIT3, TargetRegister::D);
            break;
        case PrefixOpCode::BIT_4_D:
            BIT_b_r(TargetBit::BIT4, TargetRegister::D);
            break;
        case PrefixOpCode::BIT_5_D:
            BIT_b_r(TargetBit::BIT5, TargetRegister::D);
            break;
        case PrefixOpCode::BIT_6_D:
            BIT_b_r(TargetBit::BIT6, TargetRegister::D);
            break;
        case PrefixOpCode::BIT_7_D:
            BIT_b_r(TargetBit::BIT7, TargetRegister::D);
            break;
        case PrefixOpCode::BIT_0_E:
            BIT_b_r(TargetBit::BIT0, TargetRegister::E);
            break;
        case PrefixOpCode::BIT_1_E:
            BIT_b_r(TargetBit::BIT1, TargetRegister::E);
            break;
        case PrefixOpCode::BIT_2_E:
            BIT_b_r(TargetBit::BIT2, TargetRegister::E);
            break;
        case PrefixOpCode::BIT_3_E:
            BIT_b_r(TargetBit::BIT3, TargetRegister::E);
            break;
        case PrefixOpCode::BIT_4_E:
            BIT_b_r(TargetBit::BIT4, TargetRegister::E);
            break;
        case PrefixOpCode::BIT_5_E:
            BIT_b_r(TargetBit::BIT5, TargetRegister::E);
            break;
        case PrefixOpCode::BIT_6_E:
            BIT_b_r(TargetBit::BIT6, TargetRegister::E);
            break;
        case PrefixOpCode::BIT_7_E:
            BIT_b_r(TargetBit::BIT7, TargetRegister::E);
            break;
        case PrefixOpCode::BIT_0_H:
            BIT_b_r(TargetBit::BIT0, TargetRegister::H);
            break;
        case PrefixOpCode::BIT_1_H:
            BIT_b_r(TargetBit::BIT1, TargetRegister::H);
            break;
        case PrefixOpCode::BIT_2_H:
            BIT_b_r(TargetBit::BIT2, TargetRegister::H);
            break;
        case PrefixOpCode::BIT_3_H:
            BIT_b_r(TargetBit::BIT3, TargetRegister::H);
            break;
        case PrefixOpCode::BIT_4_H:
            BIT_b_r(TargetBit::BIT4, TargetRegister::H);
            break;
        case PrefixOpCode::BIT_5_H:
            BIT_b_r(TargetBit::BIT5, TargetRegister::H);
            break;
        case PrefixOpCode::BIT_6_H:
            BIT_b_r(TargetBit::BIT6, TargetRegister::H);
            break;
        case PrefixOpCode::BIT_7_H:
            BIT_b_r(TargetBit::BIT7, TargetRegister::H);
            break;
        case PrefixOpCode::BIT_0_L:
            BIT_b_r(TargetBit::BIT0, TargetRegister::L);
            break;
        case PrefixOpCode::BIT_1_L:
            BIT_b_r(TargetBit::BIT1, TargetRegister::L);
            break;
        case PrefixOpCode::BIT_2_L:
            BIT_b_r(TargetBit::BIT2, TargetRegister::L);
            break;
        case PrefixOpCode::BIT_3_L:
            BIT_b_r(TargetBit::BIT3, TargetRegister::L);
            break;
        case PrefixOpCode::BIT_4_L:
            BIT_b_r(TargetBit::BIT4, TargetRegister::L);
            break;
        case PrefixOpCode::BIT_5_L:
            BIT_b_r(TargetBit::BIT5, TargetRegister::L);
            break;
        case PrefixOpCode::BIT_6_L:
            BIT_b_r(TargetBit::BIT6, TargetRegister::L);
            break;
        case PrefixOpCode::BIT_7_L:
            BIT_b_r(TargetBit::BIT7, TargetRegister::L);
            break;
        case PrefixOpCode::BIT_0_A:
            BIT_b_r(TargetBit::BIT0, TargetRegister::A);
            break;
        case PrefixOpCode::BIT_1_A:
            BIT_b_r(TargetBit::BIT1, TargetRegister::A);
            break;
        case PrefixOpCode::BIT_2_A:
            BIT_b_r(TargetBit::BIT2, TargetRegister::A);
            break;
        case PrefixOpCode::BIT_3_A:
            BIT_b_r(TargetBit::BIT3, TargetRegister::A);
            break;
        case PrefixOpCode::BIT_4_A:
            BIT_b_r(TargetBit::BIT4, TargetRegister::A);
            break;
        case PrefixOpCode::BIT_5_A:
            BIT_b_r(TargetBit::BIT5, TargetRegister::A);
            break;
        case PrefixOpCode::BIT_6_A:
            BIT_b_r(TargetBit::BIT6, TargetRegister::A);
            break;
        case PrefixOpCode::BIT_7_A:
            BIT_b_r(TargetBit::BIT7, TargetRegister::A);
            break;
        case PrefixOpCode::BIT_0_HL:
            BIT_b_HL(TargetBit::BIT0);
            break;
        case PrefixOpCode::BIT_1_HL:
            BIT_b_HL(TargetBit::BIT1);
            break;
        case PrefixOpCode::BIT_2_HL:
            BIT_b_HL(TargetBit::BIT2);
            break;
        case PrefixOpCode::BIT_3_HL:
            BIT_b_HL(TargetBit::BIT3);
            break;
        case PrefixOpCode::BIT_4_HL:
            BIT_b_HL(TargetBit::BIT4);
            break;
        case PrefixOpCode::BIT_5_HL:
            BIT_b_HL(TargetBit::BIT5);
            break;
        case PrefixOpCode::BIT_6_HL:
            BIT_b_HL(TargetBit::BIT6);
            break;
        case PrefixOpCode::BIT_7_HL:
            BIT_b_HL(TargetBit::BIT7);
            break;
        case PrefixOpCode::SET_0_B:
            SET_b_r(TargetBit::BIT0, TargetRegister::B);
            break;
        case PrefixOpCode::SET_1_B:
            SET_b_r(TargetBit::BIT1, TargetRegister::B);
            break;
        case PrefixOpCode::SET_2_B:
            SET_b_r(TargetBit::BIT2, TargetRegister::B);
            break;
        case PrefixOpCode::SET_3_B:
            SET_b_r(TargetBit::BIT3, TargetRegister::B);
            break;
        case PrefixOpCode::SET_4_B:
            SET_b_r(TargetBit::BIT4, TargetRegister::B);
            break;
        case PrefixOpCode::SET_5_B:
            SET_b_r(TargetBit::BIT5, TargetRegister::B);
            break;
        case PrefixOpCode::SET_6_B:
            SET_b_r(TargetBit::BIT6, TargetRegister::B);
            break;
        case PrefixOpCode::SET_7_B:
            SET_b_r(TargetBit::BIT7, TargetRegister::B);
            break;
        case PrefixOpCode::SET_0_C:
            SET_b_r(TargetBit::BIT0, TargetRegister::C);
            break;
        case PrefixOpCode::SET_1_C:
            SET_b_r(TargetBit::BIT1, TargetRegister::C);
            break;
        case PrefixOpCode::SET_2_C:
            SET_b_r(TargetBit::BIT2, TargetRegister::C);
            break;
        case PrefixOpCode::SET_3_C:
            SET_b_r(TargetBit::BIT3, TargetRegister::C);
            break;
        case PrefixOpCode::SET_4_C:
            SET_b_r(TargetBit::BIT4, TargetRegister::C);
            break;
        case PrefixOpCode::SET_5_C:
            SET_b_r(TargetBit::BIT5, TargetRegister::C);
            break;
        case PrefixOpCode::SET_6_C:
            SET_b_r(TargetBit::BIT6, TargetRegister::C);
            break;
        case PrefixOpCode::SET_7_C:
            SET_b_r(TargetBit::BIT7, TargetRegister::C);
            break;
        case PrefixOpCode::SET_0_D:
            SET_b_r(TargetBit::BIT0, TargetRegister::D);
            break;
        case PrefixOpCode::SET_1_D:
            SET_b_r(TargetBit::BIT1, TargetRegister::D);
            break;
        case PrefixOpCode::SET_2_D:
            SET_b_r(TargetBit::BIT2, TargetRegister::D);
            break;
        case PrefixOpCode::SET_3_D:
            SET_b_r(TargetBit::BIT3, TargetRegister::D);
            break;
        case PrefixOpCode::SET_4_D:
            SET_b_r(TargetBit::BIT4, TargetRegister::D);
            break;
        case PrefixOpCode::SET_5_D:
            SET_b_r(TargetBit::BIT5, TargetRegister::D);
            break;
        case PrefixOpCode::SET_6_D:
            SET_b_r(TargetBit::BIT6, TargetRegister::D);
            break;
        case PrefixOpCode::SET_7_D:
            SET_b_r(TargetBit::BIT7, TargetRegister::D);
            break;
        case PrefixOpCode::SET_0_E:
            SET_b_r(TargetBit::BIT0, TargetRegister::E);
            break;
        case PrefixOpCode::SET_1_E:
            SET_b_r(TargetBit::BIT1, TargetRegister::E);
            break;
        case PrefixOpCode::SET_2_E:
            SET_b_r(TargetBit::BIT2, TargetRegister::E);
            break;
        case PrefixOpCode::SET_3_E:
            SET_b_r(TargetBit::BIT3, TargetRegister::E);
            break;
        case PrefixOpCode::SET_4_E:
            SET_b_r(TargetBit::BIT4, TargetRegister::E);
            break;
        case PrefixOpCode::SET_5_E:
            SET_b_r(TargetBit::BIT5, TargetRegister::E);
            break;
        case PrefixOpCode::SET_6_E:
            SET_b_r(TargetBit::BIT6, TargetRegister::E);
            break;
        case PrefixOpCode::SET_7_E:
            SET_b_r(TargetBit::BIT7, TargetRegister::E);
            break;
        case PrefixOpCode::SET_0_H:
            SET_b_r(TargetBit::BIT0, TargetRegister::H);
            break;
        case PrefixOpCode::SET_1_H:
            SET_b_r(TargetBit::BIT1, TargetRegister::H);
            break;
        case PrefixOpCode::SET_2_H:
            SET_b_r(TargetBit::BIT2, TargetRegister::H);
            break;
        case PrefixOpCode::SET_3_H:
            SET_b_r(TargetBit::BIT3, TargetRegister::H);
            break;
        case PrefixOpCode::SET_4_H:
            SET_b_r(TargetBit::BIT4, TargetRegister::H);
            break;
        case PrefixOpCode::SET_5_H:
            SET_b_r(TargetBit::BIT5, TargetRegister::H);
            break;
        case PrefixOpCode::SET_6_H:
            SET_b_r(TargetBit::BIT6, TargetRegister::H);
            break;
        case PrefixOpCode::SET_7_H:
            SET_b_r(TargetBit::BIT7, TargetRegister::H);
            break;
        case PrefixOpCode::SET_0_L:
            SET_b_r(TargetBit::BIT0, TargetRegister::L);
            break;
        case PrefixOpCode::SET_1_L:
            SET_b_r(TargetBit::BIT1, TargetRegister::L);
            break;
        case PrefixOpCode::SET_2_L:
            SET_b_r(TargetBit::BIT2, TargetRegister::L);
            break;
        case PrefixOpCode::SET_3_L:
            SET_b_r(TargetBit::BIT3, TargetRegister::L);
            break;
        case PrefixOpCode::SET_4_L:
            SET_b_r(TargetBit::BIT4, TargetRegister::L);
            break;
        case PrefixOpCode::SET_5_L:
            SET_b_r(TargetBit::BIT5, TargetRegister::L);
            break;
        case PrefixOpCode::SET_6_L:
            SET_b_r(TargetBit::BIT6, TargetRegister::L);
            break;
        case PrefixOpCode::SET_7_L:
            SET_b_r(TargetBit::BIT7, TargetRegister::L);
            break;
        case PrefixOpCode::SET_0_A:
            SET_b_r(TargetBit::BIT0, TargetRegister::A);
            break;
        case PrefixOpCode::SET_1_A:
            SET_b_r(TargetBit::BIT1, TargetRegister::A);
            break;
        case PrefixOpCode::SET_2_A:
            SET_b_r(TargetBit::BIT2, TargetRegister::A);
            break;
        case PrefixOpCode::SET_3_A:
            SET_b_r(TargetBit::BIT3, TargetRegister::A);
            break;
        case PrefixOpCode::SET_4_A:
            SET_b_r(TargetBit::BIT4, TargetRegister::A);
            break;
        case PrefixOpCode::SET_5_A:
            SET_b_r(TargetBit::BIT5, TargetRegister::A);
            break;
        case PrefixOpCode::SET_6_A:
            SET_b_r(TargetBit::BIT6, TargetRegister::A);
            break;
        case PrefixOpCode::SET_7_A:
            SET_b_r(TargetBit::BIT7, TargetRegister::A);
            break;
        case PrefixOpCode::SET_0_HL:
            SET_b_HL(TargetBit::BIT0);
            break;
        case PrefixOpCode::SET_1_HL:
            SET_b_HL(TargetBit::BIT1);
            break;
        case PrefixOpCode::SET_2_HL:
            SET_b_HL(TargetBit::BIT2);
            break;
        case PrefixOpCode::SET_3_HL:
            SET_b_HL(TargetBit::BIT3);
            break;
        case PrefixOpCode::SET_4_HL:
            SET_b_HL(TargetBit::BIT4);
            break;
        case PrefixOpCode::SET_5_HL:
            SET_b_HL(TargetBit::BIT5);
            break;
        case PrefixOpCode::SET_6_HL:
            SET_b_HL(TargetBit::BIT6);
            break;
        case PrefixOpCode::SET_7_HL:
            SET_b_HL(TargetBit::BIT7);
            break;
        case PrefixOpCode::RES_0_B:
            RES_b_r(TargetBit::BIT0, TargetRegister::B);
            break;
        case PrefixOpCode::RES_1_B:
            RES_b_r(TargetBit::BIT1, TargetRegister::B);
            break;
        case PrefixOpCode::RES_2_B:
            RES_b_r(TargetBit::BIT2, TargetRegister::B);
            break;
        case PrefixOpCode::RES_3_B:
            RES_b_r(TargetBit::BIT3, TargetRegister::B);
            break;
        case PrefixOpCode::RES_4_B:
            RES_b_r(TargetBit::BIT4, TargetRegister::B);
            break;
        case PrefixOpCode::RES_5_B:
            RES_b_r(TargetBit::BIT5, TargetRegister::B);
            break;
        case PrefixOpCode::RES_6_B:
            RES_b_r(TargetBit::BIT6, TargetRegister::B);
            break;
        case PrefixOpCode::RES_7_B:
            RES_b_r(TargetBit::BIT7, TargetRegister::B);
            break;
        case PrefixOpCode::RES_0_C:
            RES_b_r(TargetBit::BIT0, TargetRegister::C);
            break;
        case PrefixOpCode::RES_1_C:
            RES_b_r(TargetBit::BIT1, TargetRegister::C);
            break;
        case PrefixOpCode::RES_2_C:
            RES_b_r(TargetBit::BIT2, TargetRegister::C);
            break;
        case PrefixOpCode::RES_3_C:
            RES_b_r(TargetBit::BIT3, TargetRegister::C);
            break;
        case PrefixOpCode::RES_4_C:
            RES_b_r(TargetBit::BIT4, TargetRegister::C);
            break;
        case PrefixOpCode::RES_5_C:
            RES_b_r(TargetBit::BIT5, TargetRegister::C);
            break;
        case PrefixOpCode::RES_6_C:
            RES_b_r(TargetBit::BIT6, TargetRegister::C);
            break;
        case PrefixOpCode::RES_7_C:
            RES_b_r(TargetBit::BIT7, TargetRegister::C);
            break;
        case PrefixOpCode::RES_0_D:
            RES_b_r(TargetBit::BIT0, TargetRegister::D);
            break;
        case PrefixOpCode::RES_1_D:
            RES_b_r(TargetBit::BIT1, TargetRegister::D);
            break;
        case PrefixOpCode::RES_2_D:
            RES_b_r(TargetBit::BIT2, TargetRegister::D);
            break;
        case PrefixOpCode::RES_3_D:
            RES_b_r(TargetBit::BIT3, TargetRegister::D);
            break;
        case PrefixOpCode::RES_4_D:
            RES_b_r(TargetBit::BIT4, TargetRegister::D);
            break;
        case PrefixOpCode::RES_5_D:
            RES_b_r(TargetBit::BIT5, TargetRegister::D);
            break;
        case PrefixOpCode::RES_6_D:
            RES_b_r(TargetBit::BIT6, TargetRegister::D);
            break;
        case PrefixOpCode::RES_7_D:
            RES_b_r(TargetBit::BIT7, TargetRegister::D);
            break;
        case PrefixOpCode::RES_0_E:
            RES_b_r(TargetBit::BIT0, TargetRegister::E);
            break;
        case PrefixOpCode::RES_1_E:
            RES_b_r(TargetBit::BIT1, TargetRegister::E);
            break;
        case PrefixOpCode::RES_2_E:
            RES_b_r(TargetBit::BIT2, TargetRegister::E);
            break;
        case PrefixOpCode::RES_3_E:
            RES_b_r(TargetBit::BIT3, TargetRegister::E);
            break;
        case PrefixOpCode::RES_4_E:
            RES_b_r(TargetBit::BIT4, TargetRegister::E);
            break;
        case PrefixOpCode::RES_5_E:
            RES_b_r(TargetBit::BIT5, TargetRegister::E);
            break;
        case PrefixOpCode::RES_6_E:
            RES_b_r(TargetBit::BIT6, TargetRegister::E);
            break;
        case PrefixOpCode::RES_7_E:
            RES_b_r(TargetBit::BIT7, TargetRegister::E);
            break;
        case PrefixOpCode::RES_0_H:
            RES_b_r(TargetBit::BIT0, TargetRegister::H);
            break;
        case PrefixOpCode::RES_1_H:
            RES_b_r(TargetBit::BIT1, TargetRegister::H);
            break;
        case PrefixOpCode::RES_2_H:
            RES_b_r(TargetBit::BIT2, TargetRegister::H);
            break;
        case PrefixOpCode::RES_3_H:
            RES_b_r(TargetBit::BIT3, TargetRegister::H);
            break;
        case PrefixOpCode::RES_4_H:
            RES_b_r(TargetBit::BIT4, TargetRegister::H);
            break;
        case PrefixOpCode::RES_5_H:
            RES_b_r(TargetBit::BIT5, TargetRegister::H);
            break;
        case PrefixOpCode::RES_6_H:
            RES_b_r(TargetBit::BIT6, TargetRegister::H);
            break;
        case PrefixOpCode::RES_7_H:
            RES_b_r(TargetBit::BIT7, TargetRegister::H);
            break;
        case PrefixOpCode::RES_0_L:
            RES_b_r(TargetBit::BIT0, TargetRegister::L);
            break;
        case PrefixOpCode::RES_1_L:
            RES_b_r(TargetBit::BIT1, TargetRegister::L);
            break;
        case PrefixOpCode::RES_2_L:
            RES_b_r(TargetBit::BIT2, TargetRegister::L);
            break;
        case PrefixOpCode::RES_3_L:
            RES_b_r(TargetBit::BIT3, TargetRegister::L);
            break;
        case PrefixOpCode::RES_4_L:
            RES_b_r(TargetBit::BIT4, TargetRegister::L);
            break;
        case PrefixOpCode::RES_5_L:
            RES_b_r(TargetBit::BIT5, TargetRegister::L);
            break;
        case PrefixOpCode::RES_6_L:
            RES_b_r(TargetBit::BIT6, TargetRegister::L);
            break;
        case PrefixOpCode::RES_7_L:
            RES_b_r(TargetBit::BIT7, TargetRegister::L);
            break;
        case PrefixOpCode::RES_0_A:
            RES_b_r(TargetBit::BIT0, TargetRegister::A);
            break;
        case PrefixOpCode::RES_1_A:
            RES_b_r(TargetBit::BIT1, TargetRegister::A);
            break;
        case PrefixOpCode::RES_2_A:
            RES_b_r(TargetBit::BIT2, TargetRegister::A);
            break;
        case PrefixOpCode::RES_3_A:
            RES_b_r(TargetBit::BIT3, TargetRegister::A);
            break;
        case PrefixOpCode::RES_4_A:
            RES_b_r(TargetBit::BIT4, TargetRegister::A);
            break;
        case PrefixOpCode::RES_5_A:
            RES_b_r(TargetBit::BIT5, TargetRegister::A);
            break;
        case PrefixOpCode::RES_6_A:
            RES_b_r(TargetBit::BIT6, TargetRegister::A);
            break;
        case PrefixOpCode::RES_7_A:
            RES_b_r(TargetBit::BIT7, TargetRegister::A);
            break;
        case PrefixOpCode::RES_0_HL:
            RES_b_HL(TargetBit::BIT0);
            break;
        case PrefixOpCode::RES_1_HL:
            RES_b_HL(TargetBit::BIT1);
            break;
        case PrefixOpCode::RES_2_HL:
            RES_b_HL(TargetBit::BIT2);
            break;
        case PrefixOpCode::RES_3_HL:
            RES_b_HL(TargetBit::BIT3);
            break;
        case PrefixOpCode::RES_4_HL:
            RES_b_HL(TargetBit::BIT4);
            break;
        case PrefixOpCode::RES_5_HL:
            RES_b_HL(TargetBit::BIT5);
            break;
        case PrefixOpCode::RES_6_HL:
            RES_b_HL(TargetBit::BIT6);
            break;
        case PrefixOpCode::RES_7_HL:
            RES_b_HL(TargetBit::BIT7);
            break;
        default:
            std::cerr << "Unknown prefix instructon: " << std::bitset<8>{static_cast<uint8_t>(current)} << ".\nTerminating bigboy...\n";
            exit(1);
    }
}

void CPU::load(uint8_t& target, uint8_t value) {
    target = value;
}

void CPU::LD_r_r(TargetRegister target, TargetRegister value) {
    load(m_registers.get(target), m_registers.get(value));
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

void CPU::ADDA_r(CPU::TargetRegister target) {
    switch (target) {
        case TargetRegister::B: add(m_registers.b); break;
        case TargetRegister::C: add(m_registers.c); break;
        case TargetRegister::D: add(m_registers.d); break;
        case TargetRegister::E: add(m_registers.e); break;
        case TargetRegister::H: add(m_registers.h); break;
        case TargetRegister::L: add(m_registers.l); break;
        case TargetRegister::A: add(m_registers.a); break;
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

void CPU::ADCA_r(CPU::TargetRegister target) {
    switch (target) {
        case TargetRegister::B: addWithCarry(m_registers.b); break;
        case TargetRegister::C: addWithCarry(m_registers.c); break;
        case TargetRegister::D: addWithCarry(m_registers.d); break;
        case TargetRegister::E: addWithCarry(m_registers.e); break;
        case TargetRegister::H: addWithCarry(m_registers.h); break;
        case TargetRegister::L: addWithCarry(m_registers.l); break;
        case TargetRegister::A: addWithCarry(m_registers.a); break;
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

void CPU::SUB_r(CPU::TargetRegister target) {
    switch (target) {
        case TargetRegister::B: subtract(m_registers.b); break;
        case TargetRegister::C: subtract(m_registers.c); break;
        case TargetRegister::D: subtract(m_registers.d); break;
        case TargetRegister::E: subtract(m_registers.e); break;
        case TargetRegister::H: subtract(m_registers.h); break;
        case TargetRegister::L: subtract(m_registers.l); break;
        case TargetRegister::A: subtract(m_registers.a); break;
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

void CPU::SBCA_r(CPU::TargetRegister target) {
    switch (target) {
        case TargetRegister::B: subtractWithCarry(m_registers.b); break;
        case TargetRegister::C: subtractWithCarry(m_registers.c); break;
        case TargetRegister::D: subtractWithCarry(m_registers.d); break;
        case TargetRegister::E: subtractWithCarry(m_registers.e); break;
        case TargetRegister::H: subtractWithCarry(m_registers.h); break;
        case TargetRegister::L: subtractWithCarry(m_registers.l); break;
        case TargetRegister::A: subtractWithCarry(m_registers.a); break;
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

void CPU::AND_r(CPU::TargetRegister target) {
    switch (target) {
        case TargetRegister::B: bitwiseAnd(m_registers.b); break;
        case TargetRegister::C: bitwiseAnd(m_registers.c); break;
        case TargetRegister::D: bitwiseAnd(m_registers.d); break;
        case TargetRegister::E: bitwiseAnd(m_registers.e); break;
        case TargetRegister::H: bitwiseAnd(m_registers.h); break;
        case TargetRegister::L: bitwiseAnd(m_registers.l); break;
        case TargetRegister::A: bitwiseAnd(m_registers.a); break;
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

void CPU::OR_r(CPU::TargetRegister target) {
    switch (target) {
        case TargetRegister::B: bitwiseOr(m_registers.b); break;
        case TargetRegister::C: bitwiseOr(m_registers.c); break;
        case TargetRegister::D: bitwiseOr(m_registers.d); break;
        case TargetRegister::E: bitwiseOr(m_registers.e); break;
        case TargetRegister::H: bitwiseOr(m_registers.h); break;
        case TargetRegister::L: bitwiseOr(m_registers.l); break;
        case TargetRegister::A: bitwiseOr(m_registers.a); break;
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

void CPU::XOR_r(CPU::TargetRegister target) {
    switch (target) {
        case TargetRegister::B: bitwiseXor(m_registers.b); break;
        case TargetRegister::C: bitwiseXor(m_registers.c); break;
        case TargetRegister::D: bitwiseXor(m_registers.d); break;
        case TargetRegister::E: bitwiseXor(m_registers.e); break;
        case TargetRegister::H: bitwiseXor(m_registers.h); break;
        case TargetRegister::L: bitwiseXor(m_registers.l); break;
        case TargetRegister::A: bitwiseXor(m_registers.a); break;
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

void CPU::CP_r(CPU::TargetRegister target) {
    switch (target) {
        case TargetRegister::B: compare(m_registers.b); break;
        case TargetRegister::C: compare(m_registers.c); break;
        case TargetRegister::D: compare(m_registers.d); break;
        case TargetRegister::E: compare(m_registers.e); break;
        case TargetRegister::H: compare(m_registers.h); break;
        case TargetRegister::L: compare(m_registers.l); break;
        case TargetRegister::A: compare(m_registers.a); break;
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

void CPU::INC_r(CPU::TargetRegister target) {
    switch (target) {
        case TargetRegister::B: increment(m_registers.b); break;
        case TargetRegister::C: increment(m_registers.c); break;
        case TargetRegister::D: increment(m_registers.d); break;
        case TargetRegister::E: increment(m_registers.e); break;
        case TargetRegister::H: increment(m_registers.h); break;
        case TargetRegister::L: increment(m_registers.l); break;
        case TargetRegister::A: increment(m_registers.a); break;
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

void CPU::DEC_r(CPU::TargetRegister target) {
    switch (target) {
        case TargetRegister::B: decrement(m_registers.b); break;
        case TargetRegister::C: decrement(m_registers.c); break;
        case TargetRegister::D: decrement(m_registers.d); break;
        case TargetRegister::E: decrement(m_registers.e); break;
        case TargetRegister::H: decrement(m_registers.h); break;
        case TargetRegister::L: decrement(m_registers.l); break;
        case TargetRegister::A: decrement(m_registers.a); break;
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

void CPU::NOP() {
    // Nothing!
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

void CPU::RLC_r(TargetRegister target) {
    switch (target) {
        case TargetRegister::B: rotateLeft(m_registers.b); break;
        case TargetRegister::C: rotateLeft(m_registers.c); break;
        case TargetRegister::D: rotateLeft(m_registers.d); break;
        case TargetRegister::E: rotateLeft(m_registers.e); break;
        case TargetRegister::H: rotateLeft(m_registers.h); break;
        case TargetRegister::L: rotateLeft(m_registers.l); break;
        case TargetRegister::A: rotateLeft(m_registers.a); break;
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

void CPU::RL_r(TargetRegister target) {
    switch (target) {
        case TargetRegister::B: rotateLeftThroughCarry(m_registers.b); break;
        case TargetRegister::C: rotateLeftThroughCarry(m_registers.c); break;
        case TargetRegister::D: rotateLeftThroughCarry(m_registers.d); break;
        case TargetRegister::E: rotateLeftThroughCarry(m_registers.e); break;
        case TargetRegister::H: rotateLeftThroughCarry(m_registers.h); break;
        case TargetRegister::L: rotateLeftThroughCarry(m_registers.l); break;
        case TargetRegister::A: rotateLeftThroughCarry(m_registers.a); break;
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

void CPU::RRC_r(TargetRegister target) {
    switch (target) {
        case TargetRegister::B: rotateRight(m_registers.b); break;
        case TargetRegister::C: rotateRight(m_registers.c); break;
        case TargetRegister::D: rotateRight(m_registers.d); break;
        case TargetRegister::E: rotateRight(m_registers.e); break;
        case TargetRegister::H: rotateRight(m_registers.h); break;
        case TargetRegister::L: rotateRight(m_registers.l); break;
        case TargetRegister::A: rotateRight(m_registers.a); break;
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

void CPU::RR_r(TargetRegister target) {
    switch (target) {
        case TargetRegister::B: rotateRightThroughCarry(m_registers.b); break;
        case TargetRegister::C: rotateRightThroughCarry(m_registers.c); break;
        case TargetRegister::D: rotateRightThroughCarry(m_registers.d); break;
        case TargetRegister::E: rotateRightThroughCarry(m_registers.e); break;
        case TargetRegister::H: rotateRightThroughCarry(m_registers.h); break;
        case TargetRegister::L: rotateRightThroughCarry(m_registers.l); break;
        case TargetRegister::A: rotateRightThroughCarry(m_registers.a); break;
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

void CPU::SLA_r(TargetRegister target) {
    switch (target) {
        case TargetRegister::B: shiftLeft(m_registers.b); break;
        case TargetRegister::C: shiftLeft(m_registers.c); break;
        case TargetRegister::D: shiftLeft(m_registers.d); break;
        case TargetRegister::E: shiftLeft(m_registers.e); break;
        case TargetRegister::H: shiftLeft(m_registers.h); break;
        case TargetRegister::L: shiftLeft(m_registers.l); break;
        case TargetRegister::A: shiftLeft(m_registers.a); break;
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

void CPU::SRA_r(TargetRegister target) {
    switch (target) {
        case TargetRegister::B: shiftTailRight(m_registers.b); break;
        case TargetRegister::C: shiftTailRight(m_registers.c); break;
        case TargetRegister::D: shiftTailRight(m_registers.d); break;
        case TargetRegister::E: shiftTailRight(m_registers.e); break;
        case TargetRegister::H: shiftTailRight(m_registers.h); break;
        case TargetRegister::L: shiftTailRight(m_registers.l); break;
        case TargetRegister::A: shiftTailRight(m_registers.a); break;
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

void CPU::SRL_r(TargetRegister target) {
    switch (target) {
        case TargetRegister::B: shiftRight(m_registers.b); break;
        case TargetRegister::C: shiftRight(m_registers.c); break;
        case TargetRegister::D: shiftRight(m_registers.d); break;
        case TargetRegister::E: shiftRight(m_registers.e); break;
        case TargetRegister::H: shiftRight(m_registers.h); break;
        case TargetRegister::L: shiftRight(m_registers.l); break;
        case TargetRegister::A: shiftRight(m_registers.a); break;
    }
}

void CPU::SRL_HL() {
    uint8_t dummy = m_bus.readByte(m_registers.getHL());
    shiftRight(dummy);
    m_bus.writeByte(m_registers.getHL(), dummy);
}

// Check if the bit at position `bit` in `byte` is zero. Set the zero flag accordingly.
void CPU::testBit(TargetBit bit, uint8_t byte) {
    uint8_t nthBit = ((byte >> static_cast<uint8_t>(bit)) & 1u);

    m_registers.f.zero = nthBit == 0;
    m_registers.f.half_carry = true;
    m_registers.f.subtract = false;
}

void CPU::BIT_b_r(TargetBit bit, TargetRegister reg) {
    switch (reg) {
        case TargetRegister::B: testBit(bit, m_registers.b); break;
        case TargetRegister::C: testBit(bit, m_registers.c); break;
        case TargetRegister::D: testBit(bit, m_registers.d); break;
        case TargetRegister::E: testBit(bit, m_registers.e); break;
        case TargetRegister::H: testBit(bit, m_registers.h); break;
        case TargetRegister::L: testBit(bit, m_registers.l); break;
        case TargetRegister::A: testBit(bit, m_registers.a); break;
    }
}

void CPU::BIT_b_HL(TargetBit bit) {
    testBit(bit, m_bus.readByte(m_registers.getHL()));
}

// Set the bit at position `bit` in `target`. No flags are affected.
void CPU::setBit(TargetBit bit, uint8_t &target) {
    target |= (1u << static_cast<uint8_t>(bit));
}

void CPU::SET_b_r(TargetBit bit, TargetRegister reg) {
    switch (reg) {
        case TargetRegister::B: setBit(bit, m_registers.b); break;
        case TargetRegister::C: setBit(bit, m_registers.c); break;
        case TargetRegister::D: setBit(bit, m_registers.d); break;
        case TargetRegister::E: setBit(bit, m_registers.e); break;
        case TargetRegister::H: setBit(bit, m_registers.h); break;
        case TargetRegister::L: setBit(bit, m_registers.l); break;
        case TargetRegister::A: setBit(bit, m_registers.a); break;
    }
}

void CPU::SET_b_HL(TargetBit bit) {
    uint8_t dummy = m_bus.readByte(m_registers.getHL());
    setBit(bit, dummy);
    m_bus.writeByte(m_registers.getHL(), dummy);
}

// Reset the bit at position `bit` in `target`. No flags are affected.
void CPU::resetBit(TargetBit bit, uint8_t &target) {
    target &= ~(1u << static_cast<uint8_t>(bit));
}

void CPU::RES_b_r(TargetBit bit, TargetRegister reg) {
    switch (reg) {
        case TargetRegister::B: resetBit(bit, m_registers.b); break;
        case TargetRegister::C: resetBit(bit, m_registers.c); break;
        case TargetRegister::D: resetBit(bit, m_registers.d); break;
        case TargetRegister::E: resetBit(bit, m_registers.e); break;
        case TargetRegister::H: resetBit(bit, m_registers.h); break;
        case TargetRegister::L: resetBit(bit, m_registers.l); break;
        case TargetRegister::A: resetBit(bit, m_registers.a); break;
    }
}

void CPU::RES_b_HL(TargetBit bit) {
    uint8_t dummy = m_bus.readByte(m_registers.getHL());
    resetBit(bit, dummy);
    m_bus.writeByte(m_registers.getHL(), dummy);
}