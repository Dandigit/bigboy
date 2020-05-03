#include <iostream>
#include <bitset>

#include <bigboy/CPU/CPU.h>

Registers& CPU::registers() {
    return m_registers;
}

const Registers& CPU::registers() const {
    return m_registers;
}

void CPU::load(const std::array<uint8_t, 0xFFFF>& memory) {
    m_mmu = MMU{memory};
    m_pc = 0;
}

void CPU::exec() {
    while (m_pc < 0xFFFF) {
        step();
    }
}

void CPU::step() {
    auto current = static_cast<OpCode>(m_mmu.readByte(m_pc++));
    switch (current) {
        case OpCode::LD_B_B:
            LD_r_r(RegisterOperand::B, RegisterOperand::B);
            break;
        case OpCode::LD_B_C:
            LD_r_r(RegisterOperand::B, RegisterOperand::C);
            break;
        case OpCode::LD_B_D:
            LD_r_r(RegisterOperand::B, RegisterOperand::D);
            break;
        case OpCode::LD_B_E:
            LD_r_r(RegisterOperand::B, RegisterOperand::E);
            break;
        case OpCode::LD_B_H:
            LD_r_r(RegisterOperand::B, RegisterOperand::H);
            break;
        case OpCode::LD_B_L:
            LD_r_r(RegisterOperand::B, RegisterOperand::L);
            break;
        case OpCode::LD_B_A:
            LD_r_r(RegisterOperand::B, RegisterOperand::A);
            break;
        case OpCode::LD_C_B:
            LD_r_r(RegisterOperand::C, RegisterOperand::B);
            break;
        case OpCode::LD_C_C:
            LD_r_r(RegisterOperand::C, RegisterOperand::C);
            break;
        case OpCode::LD_C_D:
            LD_r_r(RegisterOperand::C, RegisterOperand::D);
            break;
        case OpCode::LD_C_E:
            LD_r_r(RegisterOperand::C, RegisterOperand::E);
            break;
        case OpCode::LD_C_H:
            LD_r_r(RegisterOperand::C, RegisterOperand::H);
            break;
        case OpCode::LD_C_L:
            LD_r_r(RegisterOperand::C, RegisterOperand::L);
            break;
        case OpCode::LD_C_A:
            LD_r_r(RegisterOperand::C, RegisterOperand::A);
            break;
        case OpCode::LD_D_B:
            LD_r_r(RegisterOperand::D, RegisterOperand::B);
            break;
        case OpCode::LD_D_C:
            LD_r_r(RegisterOperand::D, RegisterOperand::C);
            break;
        case OpCode::LD_D_D:
            LD_r_r(RegisterOperand::D, RegisterOperand::D);
            break;
        case OpCode::LD_D_E:
            LD_r_r(RegisterOperand::D, RegisterOperand::E);
            break;
        case OpCode::LD_D_H:
            LD_r_r(RegisterOperand::D, RegisterOperand::H);
            break;
        case OpCode::LD_D_L:
            LD_r_r(RegisterOperand::D, RegisterOperand::L);
            break;
        case OpCode::LD_D_A:
            LD_r_r(RegisterOperand::D, RegisterOperand::A);
            break;
        case OpCode::LD_E_B:
            LD_r_r(RegisterOperand::E, RegisterOperand::B);
            break;
        case OpCode::LD_E_C:
            LD_r_r(RegisterOperand::E, RegisterOperand::C);
            break;
        case OpCode::LD_E_D:
            LD_r_r(RegisterOperand::E, RegisterOperand::D);
            break;
        case OpCode::LD_E_E:
            LD_r_r(RegisterOperand::E, RegisterOperand::E);
            break;
        case OpCode::LD_E_H:
            LD_r_r(RegisterOperand::E, RegisterOperand::H);
            break;
        case OpCode::LD_E_L:
            LD_r_r(RegisterOperand::E, RegisterOperand::L);
            break;
        case OpCode::LD_E_A:
            LD_r_r(RegisterOperand::E, RegisterOperand::A);
            break;
        case OpCode::LD_H_B:
            LD_r_r(RegisterOperand::H, RegisterOperand::B);
            break;
        case OpCode::LD_H_C:
            LD_r_r(RegisterOperand::H, RegisterOperand::C);
            break;
        case OpCode::LD_H_D:
            LD_r_r(RegisterOperand::H, RegisterOperand::D);
            break;
        case OpCode::LD_H_E:
            LD_r_r(RegisterOperand::H, RegisterOperand::E);
            break;
        case OpCode::LD_H_H:
            LD_r_r(RegisterOperand::H, RegisterOperand::H);
            break;
        case OpCode::LD_H_L:
            LD_r_r(RegisterOperand::H, RegisterOperand::L);
            break;
        case OpCode::LD_H_A:
            LD_r_r(RegisterOperand::H, RegisterOperand::A);
            break;
        case OpCode::LD_L_B:
            LD_r_r(RegisterOperand::L, RegisterOperand::B);
            break;
        case OpCode::LD_L_C:
            LD_r_r(RegisterOperand::L, RegisterOperand::C);
            break;
        case OpCode::LD_L_D:
            LD_r_r(RegisterOperand::L, RegisterOperand::D);
            break;
        case OpCode::LD_L_E:
            LD_r_r(RegisterOperand::L, RegisterOperand::E);
            break;
        case OpCode::LD_L_H:
            LD_r_r(RegisterOperand::L, RegisterOperand::H);
            break;
        case OpCode::LD_L_L:
            LD_r_r(RegisterOperand::L, RegisterOperand::L);
            break;
        case OpCode::LD_L_A:
            LD_r_r(RegisterOperand::L, RegisterOperand::A);
            break;
        case OpCode::LD_A_B:
            LD_r_r(RegisterOperand::A, RegisterOperand::B);
            break;
        case OpCode::LD_A_C:
            LD_r_r(RegisterOperand::A, RegisterOperand::C);
            break;
        case OpCode::LD_A_D:
            LD_r_r(RegisterOperand::A, RegisterOperand::D);
            break;
        case OpCode::LD_A_E:
            LD_r_r(RegisterOperand::A, RegisterOperand::E);
            break;
        case OpCode::LD_A_H:
            LD_r_r(RegisterOperand::A, RegisterOperand::H);
            break;
        case OpCode::LD_A_L:
            LD_r_r(RegisterOperand::A, RegisterOperand::L);
            break;
        case OpCode::LD_A_A:
            LD_r_r(RegisterOperand::A, RegisterOperand::A);
            break;
        case OpCode::LD_B_n:
            LD_r_n(RegisterOperand::B);
            break;
        case OpCode::LD_C_n:
            LD_r_n(RegisterOperand::C);
            break;
        case OpCode::LD_D_n:
            LD_r_n(RegisterOperand::D);
            break;
        case OpCode::LD_E_n:
            LD_r_n(RegisterOperand::E);
            break;
        case OpCode::LD_H_n:
            LD_r_n(RegisterOperand::H);
            break;
        case OpCode::LD_L_n:
            LD_r_n(RegisterOperand::L);
            break;
        case OpCode::LD_A_n:
            LD_r_n(RegisterOperand::A);
            break;
        case OpCode::LD_B_HL:
            LD_r_HL(RegisterOperand::B);
            break;
        case OpCode::LD_C_HL:
            LD_r_HL(RegisterOperand::C);
            break;
        case OpCode::LD_D_HL:
            LD_r_HL(RegisterOperand::D);
            break;
        case OpCode::LD_E_HL:
            LD_r_HL(RegisterOperand::E);
            break;
        case OpCode::LD_H_HL:
            LD_r_HL(RegisterOperand::H);
            break;
        case OpCode::LD_L_HL:
            LD_r_HL(RegisterOperand::L);
            break;
        case OpCode::LD_A_HL:
            LD_r_HL(RegisterOperand::A);
            break;
        case OpCode::LD_HL_B:
            LD_HL_r(RegisterOperand::B);
            break;
        case OpCode::LD_HL_C:
            LD_HL_r(RegisterOperand::C);
            break;
        case OpCode::LD_HL_D:
            LD_HL_r(RegisterOperand::D);
            break;
        case OpCode::LD_HL_E:
            LD_HL_r(RegisterOperand::E);
            break;
        case OpCode::LD_HL_H:
            LD_HL_r(RegisterOperand::H);
            break;
        case OpCode::LD_HL_L:
            LD_HL_r(RegisterOperand::L);
            break;
        case OpCode::LD_HL_A:
            LD_HL_r(RegisterOperand::A);
            break;
        case OpCode::LD_HL_n:
            LD_HL_n();
            break;
        case OpCode::LD_A_BC:
            LD_A_BC();
            break;
        case OpCode::LD_A_DE:
            LD_A_DE();
            break;
        case OpCode::LD_A_nn:
            LD_A_nn();
            break;
        case OpCode::LD_BC_A:
            LD_BC_A();
            break;
        case OpCode::LD_DE_A:
            LD_DE_A();
            break;
        case OpCode::LD_nn_A:
            LD_nn_A();
            break;
        case OpCode::LD_BC_nn:
            LD_dd_nn(RegisterPairOperand::BC);
            break;
        case OpCode::LD_DE_nn:
            LD_dd_nn(RegisterPairOperand::DE);
            break;
        case OpCode::LD_HL_nn:
            LD_dd_nn(RegisterPairOperand::HL);
            break;
        case OpCode::LD_SP_nn:
            LD_dd_nn(RegisterPairOperand::SP);
            break;
        case OpCode::LD_HL_mm:
            LD_HL_nn();
            break;
        case OpCode::LD_nn_HL:
            LD_nn_HL();
            break;
        case OpCode::LD_SP_HL:
            LD_SP_HL();
            break;
        case OpCode::PUSH_BC:
            PUSH_qq(RegisterPairStackOperand::BC);
            break;
        case OpCode::PUSH_DE:
            PUSH_qq(RegisterPairStackOperand::DE);
            break;
        case OpCode::PUSH_HL:
            PUSH_qq(RegisterPairStackOperand::HL);
            break;
        case OpCode::PUSH_AF:
            PUSH_qq(RegisterPairStackOperand::AF);
            break;
        case OpCode::POP_BC:
            POP_qq(RegisterPairStackOperand::BC);
            break;
        case OpCode::POP_DE:
            POP_qq(RegisterPairStackOperand::DE);
            break;
        case OpCode::POP_HL:
            POP_qq(RegisterPairStackOperand::HL);
            break;
        case OpCode::POP_AF:
            POP_qq(RegisterPairStackOperand::AF);
            break;
        case OpCode::ADDA_B:
            ADDA_r(RegisterOperand::B);
            break;
        case OpCode::ADDA_C:
            ADDA_r(RegisterOperand::C);
            break;
        case OpCode::ADDA_D:
            ADDA_r(RegisterOperand::D);
            break;
        case OpCode::ADDA_E:
            ADDA_r(RegisterOperand::E);
            break;
        case OpCode::ADDA_H:
            ADDA_r(RegisterOperand::H);
            break;
        case OpCode::ADDA_L:
            ADDA_r(RegisterOperand::L);
            break;
        case OpCode::ADDA_A:
            ADDA_r(RegisterOperand::A);
            break;
        case OpCode::ADDA_n:
            ADDA_n();
            break;
        case OpCode::ADDA_HL:
            ADDA_HL();
            break;
        case OpCode::ADCA_B:
            ADCA_r(RegisterOperand::B);
            break;
        case OpCode::ADCA_C:
            ADCA_r(RegisterOperand::C);
            break;
        case OpCode::ADCA_D:
            ADCA_r(RegisterOperand::D);
            break;
        case OpCode::ADCA_E:
            ADCA_r(RegisterOperand::E);
            break;
        case OpCode::ADCA_H:
            ADCA_r(RegisterOperand::H);
            break;
        case OpCode::ADCA_L:
            ADCA_r(RegisterOperand::L);
            break;
        case OpCode::ADCA_A:
            ADCA_r(RegisterOperand::A);
            break;
        case OpCode::ADCA_n:
            ADCA_n();
            break;
        case OpCode::ADCA_HL:
            ADCA_HL();
            break;
        case OpCode::SUB_B:
            SUB_r(RegisterOperand::B);
            break;
        case OpCode::SUB_C:
            SUB_r(RegisterOperand::C);
            break;
        case OpCode::SUB_D:
            SUB_r(RegisterOperand::D);
            break;
        case OpCode::SUB_E:
            SUB_r(RegisterOperand::E);
            break;
        case OpCode::SUB_H:
            SUB_r(RegisterOperand::H);
            break;
        case OpCode::SUB_L:
            SUB_r(RegisterOperand::L);
            break;
        case OpCode::SUB_A:
            SUB_r(RegisterOperand::A);
            break;
        case OpCode::SUB_n:
            SUB_n();
            break;
        case OpCode::SUB_HL:
            SUB_HL();
            break;
        case OpCode::SBCA_B:
            SBCA_r(RegisterOperand::B);
            break;
        case OpCode::SBCA_C:
            SBCA_r(RegisterOperand::C);
            break;
        case OpCode::SBCA_D:
            SBCA_r(RegisterOperand::D);
            break;
        case OpCode::SBCA_E:
            SBCA_r(RegisterOperand::E);
            break;
        case OpCode::SBCA_H:
            SBCA_r(RegisterOperand::H);
            break;
        case OpCode::SBCA_L:
            SBCA_r(RegisterOperand::L);
            break;
        case OpCode::SBCA_A:
            SBCA_r(RegisterOperand::A);
            break;
        case OpCode::SBCA_n:
            SBCA_n();
            break;
        case OpCode::SBCA_HL:
            SBCA_HL();
            break;
        case OpCode::AND_B:
            AND_r(RegisterOperand::B);
            break;
        case OpCode::AND_C:
            AND_r(RegisterOperand::C);
            break;
        case OpCode::AND_D:
            AND_r(RegisterOperand::D);
            break;
        case OpCode::AND_E:
            AND_r(RegisterOperand::E);
            break;
        case OpCode::AND_H:
            AND_r(RegisterOperand::H);
            break;
        case OpCode::AND_L:
            AND_r(RegisterOperand::L);
            break;
        case OpCode::AND_A:
            AND_r(RegisterOperand::A);
            break;
        case OpCode::AND_n:
            AND_n();
            break;
        case OpCode::AND_HL:
            AND_HL();
            break;
        case OpCode::OR_B:
            OR_r(RegisterOperand::B);
            break;
        case OpCode::OR_C:
            OR_r(RegisterOperand::C);
            break;
        case OpCode::OR_D:
            OR_r(RegisterOperand::D);
            break;
        case OpCode::OR_E:
            OR_r(RegisterOperand::E);
            break;
        case OpCode::OR_H:
            OR_r(RegisterOperand::H);
            break;
        case OpCode::OR_L:
            OR_r(RegisterOperand::L);
            break;
        case OpCode::OR_A:
            OR_r(RegisterOperand::A);
            break;
        case OpCode::OR_n:
            OR_n();
            break;
        case OpCode::OR_HL:
            OR_HL();
            break;
        case OpCode::XOR_B:
            XOR_r(RegisterOperand::B);
            break;
        case OpCode::XOR_C:
            XOR_r(RegisterOperand::C);
            break;
        case OpCode::XOR_D:
            XOR_r(RegisterOperand::D);
            break;
        case OpCode::XOR_E:
            XOR_r(RegisterOperand::E);
            break;
        case OpCode::XOR_H:
            XOR_r(RegisterOperand::H);
            break;
        case OpCode::XOR_L:
            XOR_r(RegisterOperand::L);
            break;
        case OpCode::XOR_A:
            XOR_r(RegisterOperand::A);
            break;
        case OpCode::XOR_n:
            XOR_n();
            break;
        case OpCode::XOR_HL:
            XOR_HL();
            break;
        case OpCode::CP_B:
            CP_r(RegisterOperand::B);
            break;
        case OpCode::CP_C:
            CP_r(RegisterOperand::C);
            break;
        case OpCode::CP_D:
            CP_r(RegisterOperand::D);
            break;
        case OpCode::CP_E:
            CP_r(RegisterOperand::E);
            break;
        case OpCode::CP_H:
            CP_r(RegisterOperand::H);
            break;
        case OpCode::CP_L:
            CP_r(RegisterOperand::L);
            break;
        case OpCode::CP_A:
            CP_r(RegisterOperand::A);
            break;
        case OpCode::CP_n:
            CP_n();
            break;
        case OpCode::CP_HL:
            CP_HL();
            break;
        case OpCode::INC_B:
            INC_r(RegisterOperand::B);
            break;
        case OpCode::INC_C:
            INC_r(RegisterOperand::C);
            break;
        case OpCode::INC_D:
            INC_r(RegisterOperand::D);
            break;
        case OpCode::INC_E:
            INC_r(RegisterOperand::E);
            break;
        case OpCode::INC_H:
            INC_r(RegisterOperand::H);
            break;
        case OpCode::INC_L:
            INC_r(RegisterOperand::L);
            break;
        case OpCode::INC_A:
            INC_r(RegisterOperand::A);
            break;
        case OpCode::INC_HL:
            INC_HL();
            break;
        case OpCode::DEC_B:
            DEC_r(RegisterOperand::B);
            break;
        case OpCode::DEC_C:
            DEC_r(RegisterOperand::C);
            break;
        case OpCode::DEC_D:
            DEC_r(RegisterOperand::D);
            break;
        case OpCode::DEC_E:
            DEC_r(RegisterOperand::E);
            break;
        case OpCode::DEC_H:
            DEC_r(RegisterOperand::H);
            break;
        case OpCode::DEC_L:
            DEC_r(RegisterOperand::L);
            break;
        case OpCode::DEC_A:
            DEC_r(RegisterOperand::A);
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
            RLC_r(RegisterOperand::B);
            break;
        case PrefixOpCode::RLC_C:
            RLC_r(RegisterOperand::C);
            break;
        case PrefixOpCode::RLC_D:
            RLC_r(RegisterOperand::D);
            break;
        case PrefixOpCode::RLC_E:
            RLC_r(RegisterOperand::E);
            break;
        case PrefixOpCode::RLC_H:
            RLC_r(RegisterOperand::H);
            break;
        case PrefixOpCode::RLC_L:
            RLC_r(RegisterOperand::L);
            break;
        case PrefixOpCode::RLC_A:
            RLC_r(RegisterOperand::A);
            break;
        case PrefixOpCode::RLC_HL:
            RLC_HL();
            break;
        case PrefixOpCode::RL_B:
            RL_r(RegisterOperand::B);
            break;
        case PrefixOpCode::RL_C:
            RL_r(RegisterOperand::C);
            break;
        case PrefixOpCode::RL_D:
            RL_r(RegisterOperand::D);
            break;
        case PrefixOpCode::RL_E:
            RL_r(RegisterOperand::E);
            break;
        case PrefixOpCode::RL_H:
            RL_r(RegisterOperand::H);
            break;
        case PrefixOpCode::RL_L:
            RL_r(RegisterOperand::L);
            break;
        case PrefixOpCode::RL_A:
            RL_r(RegisterOperand::A);
            break;
        case PrefixOpCode::RL_HL:
            RL_HL();
            break;
        case PrefixOpCode::RRC_B:
            RRC_r(RegisterOperand::B);
            break;
        case PrefixOpCode::RRC_C:
            RRC_r(RegisterOperand::C);
            break;
        case PrefixOpCode::RRC_D:
            RRC_r(RegisterOperand::D);
            break;
        case PrefixOpCode::RRC_E:
            RRC_r(RegisterOperand::E);
            break;
        case PrefixOpCode::RRC_H:
            RRC_r(RegisterOperand::H);
            break;
        case PrefixOpCode::RRC_L:
            RRC_r(RegisterOperand::L);
            break;
        case PrefixOpCode::RRC_A:
            RRC_r(RegisterOperand::A);
            break;
        case PrefixOpCode::RRC_HL:
            RRC_HL();
            break;
        case PrefixOpCode::RR_B:
            RR_r(RegisterOperand::B);
            break;
        case PrefixOpCode::RR_C:
            RR_r(RegisterOperand::C);
            break;
        case PrefixOpCode::RR_D:
            RR_r(RegisterOperand::D);
            break;
        case PrefixOpCode::RR_E:
            RR_r(RegisterOperand::E);
            break;
        case PrefixOpCode::RR_H:
            RR_r(RegisterOperand::H);
            break;
        case PrefixOpCode::RR_L:
            RR_r(RegisterOperand::L);
            break;
        case PrefixOpCode::RR_A:
            RR_r(RegisterOperand::A);
            break;
        case PrefixOpCode::RR_HL:
            RR_HL();
            break;
        case PrefixOpCode::SLA_B:
            SLA_r(RegisterOperand::B);
            break;
        case PrefixOpCode::SLA_C:
            SLA_r(RegisterOperand::C);
            break;
        case PrefixOpCode::SLA_D:
            SLA_r(RegisterOperand::D);
            break;
        case PrefixOpCode::SLA_E:
            SLA_r(RegisterOperand::E);
            break;
        case PrefixOpCode::SLA_H:
            SLA_r(RegisterOperand::H);
            break;
        case PrefixOpCode::SLA_L:
            SLA_r(RegisterOperand::L);
            break;
        case PrefixOpCode::SLA_A:
            SLA_r(RegisterOperand::A);
            break;
        case PrefixOpCode::SLA_HL:
            SLA_HL();
            break;
        case PrefixOpCode::SRA_B:
            SRA_r(RegisterOperand::B);
            break;
        case PrefixOpCode::SRA_C:
            SRA_r(RegisterOperand::C);
            break;
        case PrefixOpCode::SRA_D:
            SRA_r(RegisterOperand::D);
            break;
        case PrefixOpCode::SRA_E:
            SRA_r(RegisterOperand::E);
            break;
        case PrefixOpCode::SRA_H:
            SRA_r(RegisterOperand::H);
            break;
        case PrefixOpCode::SRA_L:
            SRA_r(RegisterOperand::L);
            break;
        case PrefixOpCode::SRA_A:
            SRA_r(RegisterOperand::A);
            break;
        case PrefixOpCode::SRA_HL:
            SRA_HL();
            break;
        case PrefixOpCode::SRL_B:
            SRL_r(RegisterOperand::B);
            break;
        case PrefixOpCode::SRL_C:
            SRL_r(RegisterOperand::C);
            break;
        case PrefixOpCode::SRL_D:
            SRL_r(RegisterOperand::D);
            break;
        case PrefixOpCode::SRL_E:
            SRL_r(RegisterOperand::E);
            break;
        case PrefixOpCode::SRL_H:
            SRL_r(RegisterOperand::H);
            break;
        case PrefixOpCode::SRL_L:
            SRL_r(RegisterOperand::L);
            break;
        case PrefixOpCode::SRL_A:
            SRL_r(RegisterOperand::A);
            break;
        case PrefixOpCode::SRL_HL:
            SRL_HL();
            break;
        case PrefixOpCode::BIT_0_B:
            BIT_b_r(BitOperand::BIT0, RegisterOperand::B);
            break;
        case PrefixOpCode::BIT_1_B:
            BIT_b_r(BitOperand::BIT1, RegisterOperand::B);
            break;
        case PrefixOpCode::BIT_2_B:
            BIT_b_r(BitOperand::BIT2, RegisterOperand::B);
            break;
        case PrefixOpCode::BIT_3_B:
            BIT_b_r(BitOperand::BIT3, RegisterOperand::B);
            break;
        case PrefixOpCode::BIT_4_B:
            BIT_b_r(BitOperand::BIT4, RegisterOperand::B);
            break;
        case PrefixOpCode::BIT_5_B:
            BIT_b_r(BitOperand::BIT5, RegisterOperand::B);
            break;
        case PrefixOpCode::BIT_6_B:
            BIT_b_r(BitOperand::BIT6, RegisterOperand::B);
            break;
        case PrefixOpCode::BIT_7_B:
            BIT_b_r(BitOperand::BIT7, RegisterOperand::B);
            break;
        case PrefixOpCode::BIT_0_C:
            BIT_b_r(BitOperand::BIT0, RegisterOperand::C);
            break;
        case PrefixOpCode::BIT_1_C:
            BIT_b_r(BitOperand::BIT1, RegisterOperand::C);
            break;
        case PrefixOpCode::BIT_2_C:
            BIT_b_r(BitOperand::BIT2, RegisterOperand::C);
            break;
        case PrefixOpCode::BIT_3_C:
            BIT_b_r(BitOperand::BIT3, RegisterOperand::C);
            break;
        case PrefixOpCode::BIT_4_C:
            BIT_b_r(BitOperand::BIT4, RegisterOperand::C);
            break;
        case PrefixOpCode::BIT_5_C:
            BIT_b_r(BitOperand::BIT5, RegisterOperand::C);
            break;
        case PrefixOpCode::BIT_6_C:
            BIT_b_r(BitOperand::BIT6, RegisterOperand::C);
            break;
        case PrefixOpCode::BIT_7_C:
            BIT_b_r(BitOperand::BIT7, RegisterOperand::C);
            break;
        case PrefixOpCode::BIT_0_D:
            BIT_b_r(BitOperand::BIT0, RegisterOperand::D);
            break;
        case PrefixOpCode::BIT_1_D:
            BIT_b_r(BitOperand::BIT1, RegisterOperand::D);
            break;
        case PrefixOpCode::BIT_2_D:
            BIT_b_r(BitOperand::BIT2, RegisterOperand::D);
            break;
        case PrefixOpCode::BIT_3_D:
            BIT_b_r(BitOperand::BIT3, RegisterOperand::D);
            break;
        case PrefixOpCode::BIT_4_D:
            BIT_b_r(BitOperand::BIT4, RegisterOperand::D);
            break;
        case PrefixOpCode::BIT_5_D:
            BIT_b_r(BitOperand::BIT5, RegisterOperand::D);
            break;
        case PrefixOpCode::BIT_6_D:
            BIT_b_r(BitOperand::BIT6, RegisterOperand::D);
            break;
        case PrefixOpCode::BIT_7_D:
            BIT_b_r(BitOperand::BIT7, RegisterOperand::D);
            break;
        case PrefixOpCode::BIT_0_E:
            BIT_b_r(BitOperand::BIT0, RegisterOperand::E);
            break;
        case PrefixOpCode::BIT_1_E:
            BIT_b_r(BitOperand::BIT1, RegisterOperand::E);
            break;
        case PrefixOpCode::BIT_2_E:
            BIT_b_r(BitOperand::BIT2, RegisterOperand::E);
            break;
        case PrefixOpCode::BIT_3_E:
            BIT_b_r(BitOperand::BIT3, RegisterOperand::E);
            break;
        case PrefixOpCode::BIT_4_E:
            BIT_b_r(BitOperand::BIT4, RegisterOperand::E);
            break;
        case PrefixOpCode::BIT_5_E:
            BIT_b_r(BitOperand::BIT5, RegisterOperand::E);
            break;
        case PrefixOpCode::BIT_6_E:
            BIT_b_r(BitOperand::BIT6, RegisterOperand::E);
            break;
        case PrefixOpCode::BIT_7_E:
            BIT_b_r(BitOperand::BIT7, RegisterOperand::E);
            break;
        case PrefixOpCode::BIT_0_H:
            BIT_b_r(BitOperand::BIT0, RegisterOperand::H);
            break;
        case PrefixOpCode::BIT_1_H:
            BIT_b_r(BitOperand::BIT1, RegisterOperand::H);
            break;
        case PrefixOpCode::BIT_2_H:
            BIT_b_r(BitOperand::BIT2, RegisterOperand::H);
            break;
        case PrefixOpCode::BIT_3_H:
            BIT_b_r(BitOperand::BIT3, RegisterOperand::H);
            break;
        case PrefixOpCode::BIT_4_H:
            BIT_b_r(BitOperand::BIT4, RegisterOperand::H);
            break;
        case PrefixOpCode::BIT_5_H:
            BIT_b_r(BitOperand::BIT5, RegisterOperand::H);
            break;
        case PrefixOpCode::BIT_6_H:
            BIT_b_r(BitOperand::BIT6, RegisterOperand::H);
            break;
        case PrefixOpCode::BIT_7_H:
            BIT_b_r(BitOperand::BIT7, RegisterOperand::H);
            break;
        case PrefixOpCode::BIT_0_L:
            BIT_b_r(BitOperand::BIT0, RegisterOperand::L);
            break;
        case PrefixOpCode::BIT_1_L:
            BIT_b_r(BitOperand::BIT1, RegisterOperand::L);
            break;
        case PrefixOpCode::BIT_2_L:
            BIT_b_r(BitOperand::BIT2, RegisterOperand::L);
            break;
        case PrefixOpCode::BIT_3_L:
            BIT_b_r(BitOperand::BIT3, RegisterOperand::L);
            break;
        case PrefixOpCode::BIT_4_L:
            BIT_b_r(BitOperand::BIT4, RegisterOperand::L);
            break;
        case PrefixOpCode::BIT_5_L:
            BIT_b_r(BitOperand::BIT5, RegisterOperand::L);
            break;
        case PrefixOpCode::BIT_6_L:
            BIT_b_r(BitOperand::BIT6, RegisterOperand::L);
            break;
        case PrefixOpCode::BIT_7_L:
            BIT_b_r(BitOperand::BIT7, RegisterOperand::L);
            break;
        case PrefixOpCode::BIT_0_A:
            BIT_b_r(BitOperand::BIT0, RegisterOperand::A);
            break;
        case PrefixOpCode::BIT_1_A:
            BIT_b_r(BitOperand::BIT1, RegisterOperand::A);
            break;
        case PrefixOpCode::BIT_2_A:
            BIT_b_r(BitOperand::BIT2, RegisterOperand::A);
            break;
        case PrefixOpCode::BIT_3_A:
            BIT_b_r(BitOperand::BIT3, RegisterOperand::A);
            break;
        case PrefixOpCode::BIT_4_A:
            BIT_b_r(BitOperand::BIT4, RegisterOperand::A);
            break;
        case PrefixOpCode::BIT_5_A:
            BIT_b_r(BitOperand::BIT5, RegisterOperand::A);
            break;
        case PrefixOpCode::BIT_6_A:
            BIT_b_r(BitOperand::BIT6, RegisterOperand::A);
            break;
        case PrefixOpCode::BIT_7_A:
            BIT_b_r(BitOperand::BIT7, RegisterOperand::A);
            break;
        case PrefixOpCode::BIT_0_HL:
            BIT_b_HL(BitOperand::BIT0);
            break;
        case PrefixOpCode::BIT_1_HL:
            BIT_b_HL(BitOperand::BIT1);
            break;
        case PrefixOpCode::BIT_2_HL:
            BIT_b_HL(BitOperand::BIT2);
            break;
        case PrefixOpCode::BIT_3_HL:
            BIT_b_HL(BitOperand::BIT3);
            break;
        case PrefixOpCode::BIT_4_HL:
            BIT_b_HL(BitOperand::BIT4);
            break;
        case PrefixOpCode::BIT_5_HL:
            BIT_b_HL(BitOperand::BIT5);
            break;
        case PrefixOpCode::BIT_6_HL:
            BIT_b_HL(BitOperand::BIT6);
            break;
        case PrefixOpCode::BIT_7_HL:
            BIT_b_HL(BitOperand::BIT7);
            break;
        case PrefixOpCode::SET_0_B:
            SET_b_r(BitOperand::BIT0, RegisterOperand::B);
            break;
        case PrefixOpCode::SET_1_B:
            SET_b_r(BitOperand::BIT1, RegisterOperand::B);
            break;
        case PrefixOpCode::SET_2_B:
            SET_b_r(BitOperand::BIT2, RegisterOperand::B);
            break;
        case PrefixOpCode::SET_3_B:
            SET_b_r(BitOperand::BIT3, RegisterOperand::B);
            break;
        case PrefixOpCode::SET_4_B:
            SET_b_r(BitOperand::BIT4, RegisterOperand::B);
            break;
        case PrefixOpCode::SET_5_B:
            SET_b_r(BitOperand::BIT5, RegisterOperand::B);
            break;
        case PrefixOpCode::SET_6_B:
            SET_b_r(BitOperand::BIT6, RegisterOperand::B);
            break;
        case PrefixOpCode::SET_7_B:
            SET_b_r(BitOperand::BIT7, RegisterOperand::B);
            break;
        case PrefixOpCode::SET_0_C:
            SET_b_r(BitOperand::BIT0, RegisterOperand::C);
            break;
        case PrefixOpCode::SET_1_C:
            SET_b_r(BitOperand::BIT1, RegisterOperand::C);
            break;
        case PrefixOpCode::SET_2_C:
            SET_b_r(BitOperand::BIT2, RegisterOperand::C);
            break;
        case PrefixOpCode::SET_3_C:
            SET_b_r(BitOperand::BIT3, RegisterOperand::C);
            break;
        case PrefixOpCode::SET_4_C:
            SET_b_r(BitOperand::BIT4, RegisterOperand::C);
            break;
        case PrefixOpCode::SET_5_C:
            SET_b_r(BitOperand::BIT5, RegisterOperand::C);
            break;
        case PrefixOpCode::SET_6_C:
            SET_b_r(BitOperand::BIT6, RegisterOperand::C);
            break;
        case PrefixOpCode::SET_7_C:
            SET_b_r(BitOperand::BIT7, RegisterOperand::C);
            break;
        case PrefixOpCode::SET_0_D:
            SET_b_r(BitOperand::BIT0, RegisterOperand::D);
            break;
        case PrefixOpCode::SET_1_D:
            SET_b_r(BitOperand::BIT1, RegisterOperand::D);
            break;
        case PrefixOpCode::SET_2_D:
            SET_b_r(BitOperand::BIT2, RegisterOperand::D);
            break;
        case PrefixOpCode::SET_3_D:
            SET_b_r(BitOperand::BIT3, RegisterOperand::D);
            break;
        case PrefixOpCode::SET_4_D:
            SET_b_r(BitOperand::BIT4, RegisterOperand::D);
            break;
        case PrefixOpCode::SET_5_D:
            SET_b_r(BitOperand::BIT5, RegisterOperand::D);
            break;
        case PrefixOpCode::SET_6_D:
            SET_b_r(BitOperand::BIT6, RegisterOperand::D);
            break;
        case PrefixOpCode::SET_7_D:
            SET_b_r(BitOperand::BIT7, RegisterOperand::D);
            break;
        case PrefixOpCode::SET_0_E:
            SET_b_r(BitOperand::BIT0, RegisterOperand::E);
            break;
        case PrefixOpCode::SET_1_E:
            SET_b_r(BitOperand::BIT1, RegisterOperand::E);
            break;
        case PrefixOpCode::SET_2_E:
            SET_b_r(BitOperand::BIT2, RegisterOperand::E);
            break;
        case PrefixOpCode::SET_3_E:
            SET_b_r(BitOperand::BIT3, RegisterOperand::E);
            break;
        case PrefixOpCode::SET_4_E:
            SET_b_r(BitOperand::BIT4, RegisterOperand::E);
            break;
        case PrefixOpCode::SET_5_E:
            SET_b_r(BitOperand::BIT5, RegisterOperand::E);
            break;
        case PrefixOpCode::SET_6_E:
            SET_b_r(BitOperand::BIT6, RegisterOperand::E);
            break;
        case PrefixOpCode::SET_7_E:
            SET_b_r(BitOperand::BIT7, RegisterOperand::E);
            break;
        case PrefixOpCode::SET_0_H:
            SET_b_r(BitOperand::BIT0, RegisterOperand::H);
            break;
        case PrefixOpCode::SET_1_H:
            SET_b_r(BitOperand::BIT1, RegisterOperand::H);
            break;
        case PrefixOpCode::SET_2_H:
            SET_b_r(BitOperand::BIT2, RegisterOperand::H);
            break;
        case PrefixOpCode::SET_3_H:
            SET_b_r(BitOperand::BIT3, RegisterOperand::H);
            break;
        case PrefixOpCode::SET_4_H:
            SET_b_r(BitOperand::BIT4, RegisterOperand::H);
            break;
        case PrefixOpCode::SET_5_H:
            SET_b_r(BitOperand::BIT5, RegisterOperand::H);
            break;
        case PrefixOpCode::SET_6_H:
            SET_b_r(BitOperand::BIT6, RegisterOperand::H);
            break;
        case PrefixOpCode::SET_7_H:
            SET_b_r(BitOperand::BIT7, RegisterOperand::H);
            break;
        case PrefixOpCode::SET_0_L:
            SET_b_r(BitOperand::BIT0, RegisterOperand::L);
            break;
        case PrefixOpCode::SET_1_L:
            SET_b_r(BitOperand::BIT1, RegisterOperand::L);
            break;
        case PrefixOpCode::SET_2_L:
            SET_b_r(BitOperand::BIT2, RegisterOperand::L);
            break;
        case PrefixOpCode::SET_3_L:
            SET_b_r(BitOperand::BIT3, RegisterOperand::L);
            break;
        case PrefixOpCode::SET_4_L:
            SET_b_r(BitOperand::BIT4, RegisterOperand::L);
            break;
        case PrefixOpCode::SET_5_L:
            SET_b_r(BitOperand::BIT5, RegisterOperand::L);
            break;
        case PrefixOpCode::SET_6_L:
            SET_b_r(BitOperand::BIT6, RegisterOperand::L);
            break;
        case PrefixOpCode::SET_7_L:
            SET_b_r(BitOperand::BIT7, RegisterOperand::L);
            break;
        case PrefixOpCode::SET_0_A:
            SET_b_r(BitOperand::BIT0, RegisterOperand::A);
            break;
        case PrefixOpCode::SET_1_A:
            SET_b_r(BitOperand::BIT1, RegisterOperand::A);
            break;
        case PrefixOpCode::SET_2_A:
            SET_b_r(BitOperand::BIT2, RegisterOperand::A);
            break;
        case PrefixOpCode::SET_3_A:
            SET_b_r(BitOperand::BIT3, RegisterOperand::A);
            break;
        case PrefixOpCode::SET_4_A:
            SET_b_r(BitOperand::BIT4, RegisterOperand::A);
            break;
        case PrefixOpCode::SET_5_A:
            SET_b_r(BitOperand::BIT5, RegisterOperand::A);
            break;
        case PrefixOpCode::SET_6_A:
            SET_b_r(BitOperand::BIT6, RegisterOperand::A);
            break;
        case PrefixOpCode::SET_7_A:
            SET_b_r(BitOperand::BIT7, RegisterOperand::A);
            break;
        case PrefixOpCode::SET_0_HL:
            SET_b_HL(BitOperand::BIT0);
            break;
        case PrefixOpCode::SET_1_HL:
            SET_b_HL(BitOperand::BIT1);
            break;
        case PrefixOpCode::SET_2_HL:
            SET_b_HL(BitOperand::BIT2);
            break;
        case PrefixOpCode::SET_3_HL:
            SET_b_HL(BitOperand::BIT3);
            break;
        case PrefixOpCode::SET_4_HL:
            SET_b_HL(BitOperand::BIT4);
            break;
        case PrefixOpCode::SET_5_HL:
            SET_b_HL(BitOperand::BIT5);
            break;
        case PrefixOpCode::SET_6_HL:
            SET_b_HL(BitOperand::BIT6);
            break;
        case PrefixOpCode::SET_7_HL:
            SET_b_HL(BitOperand::BIT7);
            break;
        case PrefixOpCode::RES_0_B:
            RES_b_r(BitOperand::BIT0, RegisterOperand::B);
            break;
        case PrefixOpCode::RES_1_B:
            RES_b_r(BitOperand::BIT1, RegisterOperand::B);
            break;
        case PrefixOpCode::RES_2_B:
            RES_b_r(BitOperand::BIT2, RegisterOperand::B);
            break;
        case PrefixOpCode::RES_3_B:
            RES_b_r(BitOperand::BIT3, RegisterOperand::B);
            break;
        case PrefixOpCode::RES_4_B:
            RES_b_r(BitOperand::BIT4, RegisterOperand::B);
            break;
        case PrefixOpCode::RES_5_B:
            RES_b_r(BitOperand::BIT5, RegisterOperand::B);
            break;
        case PrefixOpCode::RES_6_B:
            RES_b_r(BitOperand::BIT6, RegisterOperand::B);
            break;
        case PrefixOpCode::RES_7_B:
            RES_b_r(BitOperand::BIT7, RegisterOperand::B);
            break;
        case PrefixOpCode::RES_0_C:
            RES_b_r(BitOperand::BIT0, RegisterOperand::C);
            break;
        case PrefixOpCode::RES_1_C:
            RES_b_r(BitOperand::BIT1, RegisterOperand::C);
            break;
        case PrefixOpCode::RES_2_C:
            RES_b_r(BitOperand::BIT2, RegisterOperand::C);
            break;
        case PrefixOpCode::RES_3_C:
            RES_b_r(BitOperand::BIT3, RegisterOperand::C);
            break;
        case PrefixOpCode::RES_4_C:
            RES_b_r(BitOperand::BIT4, RegisterOperand::C);
            break;
        case PrefixOpCode::RES_5_C:
            RES_b_r(BitOperand::BIT5, RegisterOperand::C);
            break;
        case PrefixOpCode::RES_6_C:
            RES_b_r(BitOperand::BIT6, RegisterOperand::C);
            break;
        case PrefixOpCode::RES_7_C:
            RES_b_r(BitOperand::BIT7, RegisterOperand::C);
            break;
        case PrefixOpCode::RES_0_D:
            RES_b_r(BitOperand::BIT0, RegisterOperand::D);
            break;
        case PrefixOpCode::RES_1_D:
            RES_b_r(BitOperand::BIT1, RegisterOperand::D);
            break;
        case PrefixOpCode::RES_2_D:
            RES_b_r(BitOperand::BIT2, RegisterOperand::D);
            break;
        case PrefixOpCode::RES_3_D:
            RES_b_r(BitOperand::BIT3, RegisterOperand::D);
            break;
        case PrefixOpCode::RES_4_D:
            RES_b_r(BitOperand::BIT4, RegisterOperand::D);
            break;
        case PrefixOpCode::RES_5_D:
            RES_b_r(BitOperand::BIT5, RegisterOperand::D);
            break;
        case PrefixOpCode::RES_6_D:
            RES_b_r(BitOperand::BIT6, RegisterOperand::D);
            break;
        case PrefixOpCode::RES_7_D:
            RES_b_r(BitOperand::BIT7, RegisterOperand::D);
            break;
        case PrefixOpCode::RES_0_E:
            RES_b_r(BitOperand::BIT0, RegisterOperand::E);
            break;
        case PrefixOpCode::RES_1_E:
            RES_b_r(BitOperand::BIT1, RegisterOperand::E);
            break;
        case PrefixOpCode::RES_2_E:
            RES_b_r(BitOperand::BIT2, RegisterOperand::E);
            break;
        case PrefixOpCode::RES_3_E:
            RES_b_r(BitOperand::BIT3, RegisterOperand::E);
            break;
        case PrefixOpCode::RES_4_E:
            RES_b_r(BitOperand::BIT4, RegisterOperand::E);
            break;
        case PrefixOpCode::RES_5_E:
            RES_b_r(BitOperand::BIT5, RegisterOperand::E);
            break;
        case PrefixOpCode::RES_6_E:
            RES_b_r(BitOperand::BIT6, RegisterOperand::E);
            break;
        case PrefixOpCode::RES_7_E:
            RES_b_r(BitOperand::BIT7, RegisterOperand::E);
            break;
        case PrefixOpCode::RES_0_H:
            RES_b_r(BitOperand::BIT0, RegisterOperand::H);
            break;
        case PrefixOpCode::RES_1_H:
            RES_b_r(BitOperand::BIT1, RegisterOperand::H);
            break;
        case PrefixOpCode::RES_2_H:
            RES_b_r(BitOperand::BIT2, RegisterOperand::H);
            break;
        case PrefixOpCode::RES_3_H:
            RES_b_r(BitOperand::BIT3, RegisterOperand::H);
            break;
        case PrefixOpCode::RES_4_H:
            RES_b_r(BitOperand::BIT4, RegisterOperand::H);
            break;
        case PrefixOpCode::RES_5_H:
            RES_b_r(BitOperand::BIT5, RegisterOperand::H);
            break;
        case PrefixOpCode::RES_6_H:
            RES_b_r(BitOperand::BIT6, RegisterOperand::H);
            break;
        case PrefixOpCode::RES_7_H:
            RES_b_r(BitOperand::BIT7, RegisterOperand::H);
            break;
        case PrefixOpCode::RES_0_L:
            RES_b_r(BitOperand::BIT0, RegisterOperand::L);
            break;
        case PrefixOpCode::RES_1_L:
            RES_b_r(BitOperand::BIT1, RegisterOperand::L);
            break;
        case PrefixOpCode::RES_2_L:
            RES_b_r(BitOperand::BIT2, RegisterOperand::L);
            break;
        case PrefixOpCode::RES_3_L:
            RES_b_r(BitOperand::BIT3, RegisterOperand::L);
            break;
        case PrefixOpCode::RES_4_L:
            RES_b_r(BitOperand::BIT4, RegisterOperand::L);
            break;
        case PrefixOpCode::RES_5_L:
            RES_b_r(BitOperand::BIT5, RegisterOperand::L);
            break;
        case PrefixOpCode::RES_6_L:
            RES_b_r(BitOperand::BIT6, RegisterOperand::L);
            break;
        case PrefixOpCode::RES_7_L:
            RES_b_r(BitOperand::BIT7, RegisterOperand::L);
            break;
        case PrefixOpCode::RES_0_A:
            RES_b_r(BitOperand::BIT0, RegisterOperand::A);
            break;
        case PrefixOpCode::RES_1_A:
            RES_b_r(BitOperand::BIT1, RegisterOperand::A);
            break;
        case PrefixOpCode::RES_2_A:
            RES_b_r(BitOperand::BIT2, RegisterOperand::A);
            break;
        case PrefixOpCode::RES_3_A:
            RES_b_r(BitOperand::BIT3, RegisterOperand::A);
            break;
        case PrefixOpCode::RES_4_A:
            RES_b_r(BitOperand::BIT4, RegisterOperand::A);
            break;
        case PrefixOpCode::RES_5_A:
            RES_b_r(BitOperand::BIT5, RegisterOperand::A);
            break;
        case PrefixOpCode::RES_6_A:
            RES_b_r(BitOperand::BIT6, RegisterOperand::A);
            break;
        case PrefixOpCode::RES_7_A:
            RES_b_r(BitOperand::BIT7, RegisterOperand::A);
            break;
        case PrefixOpCode::RES_0_HL:
            RES_b_HL(BitOperand::BIT0);
            break;
        case PrefixOpCode::RES_1_HL:
            RES_b_HL(BitOperand::BIT1);
            break;
        case PrefixOpCode::RES_2_HL:
            RES_b_HL(BitOperand::BIT2);
            break;
        case PrefixOpCode::RES_3_HL:
            RES_b_HL(BitOperand::BIT3);
            break;
        case PrefixOpCode::RES_4_HL:
            RES_b_HL(BitOperand::BIT4);
            break;
        case PrefixOpCode::RES_5_HL:
            RES_b_HL(BitOperand::BIT5);
            break;
        case PrefixOpCode::RES_6_HL:
            RES_b_HL(BitOperand::BIT6);
            break;
        case PrefixOpCode::RES_7_HL:
            RES_b_HL(BitOperand::BIT7);
            break;
        default:
            std::cerr << "Unknown prefix instructon: " << std::bitset<8>{static_cast<uint8_t>(current)} << ".\nTerminating bigboy...\n";
            exit(1);
    }
}

void CPU::load(uint8_t& target, uint8_t value) {
    target = value;
}

void CPU::LD_r_r(RegisterOperand target, RegisterOperand value) {
    load(m_registers.get(target), m_registers.get(value));
}

void CPU::LD_r_n(RegisterOperand target) {
    load(m_registers.get(target), m_mmu.readByte(m_pc++));
}

void CPU::LD_r_HL(RegisterOperand target) {
    load(m_registers.get(target), m_mmu.readByte(m_registers.getHL()));
}

void CPU::LD_HL_r(RegisterOperand value) {
    uint8_t dummy = m_mmu.readByte(m_registers.getHL());
    load(dummy, m_registers.get(value));
    m_mmu.writeByte(m_registers.getHL(), dummy);
}

void CPU::LD_HL_n() {
    uint8_t dummy = m_mmu.readByte(m_registers.getHL());
    load(dummy, m_mmu.readByte(m_pc++));
    m_mmu.writeByte(m_registers.getHL(), dummy);
}

void CPU::LD_A_BC() {
    load(m_registers.a, m_mmu.readByte(m_registers.getBC()));
}

void CPU::LD_A_DE() {
    load(m_registers.a, m_mmu.readByte(m_registers.getDE()));
}

void CPU::LD_A_nn() {
    uint8_t lower = m_mmu.readByte(m_pc++);
    uint8_t higher = m_mmu.readByte(m_pc++);
    uint16_t nn = (higher << 8u) | lower;

    load(m_registers.a, m_mmu.readByte(nn));
}

void CPU::LD_BC_A() {
    load(m_mmu.byteAt(m_registers.getBC()), m_registers.a);
}

void CPU::LD_DE_A() {
    load(m_mmu.byteAt(m_registers.getDE()), m_registers.a);
}

void CPU::LD_nn_A() {
    uint8_t lower = m_mmu.readByte(m_pc++);
    uint8_t higher = m_mmu.readByte(m_pc++);
    uint16_t nn = (higher << 8u) | lower;

    load(m_mmu.byteAt(nn), m_registers.a);
}

void CPU::load(uint16_t& target, uint16_t value) {
    target = value;
}

void CPU::LD_dd_nn(RegisterPairOperand target) {
    uint8_t lower = m_mmu.readByte(m_pc++);
    uint8_t higher = m_mmu.readByte(m_pc++);
    uint16_t nn = (higher << 8u) | lower;

    load(m_registers.get(target), nn);
}

void CPU::LD_HL_nn() {
    uint8_t lower = m_mmu.readByte(m_pc++);
    uint8_t higher = m_mmu.readByte(m_pc++);
    uint16_t nn = (higher << 8u) | lower;

    load(m_registers.l, m_mmu.byteAt(nn));
    load(m_registers.h, m_mmu.byteAt(nn + 1));
}

void CPU::LD_nn_HL() {
    uint8_t lower = m_mmu.readByte(m_pc++);
    uint8_t higher = m_mmu.readByte(m_pc++);
    uint16_t nn = (higher << 8u) | lower;

    load(m_mmu.byteAt(nn), m_registers.l);
    load(m_mmu.byteAt(nn + 1), m_registers.h);
}

void CPU::LD_SP_HL() {
    load(m_registers.sp, m_registers.HL());
}

void CPU::push(uint16_t value) {
    uint8_t high = (value >> 8u);
    m_mmu.byteAt(--m_registers.sp) = high;

    uint8_t low = (value & 0xFFu);
    m_mmu.byteAt(--m_registers.sp) = low;
}

void CPU::PUSH_qq(RegisterPairStackOperand value) {
    push(m_registers.get(value));
}

void CPU::pop(uint16_t& target) {
    uint8_t low = m_mmu.byteAt(m_registers.sp--);
    uint8_t high = m_mmu.byteAt(m_registers.sp--);

    target = (high << 8u) | low;
}

void CPU::POP_qq(RegisterPairStackOperand target) {
    pop(m_registers.get(target));
}

// Add `value` to the register A, and set/reset the necessary flags
void CPU::add(uint8_t value) {
    uint8_t result = m_registers.a + value;

    m_flags.zero = result == 0;
    m_flags.subtract = false;
    m_flags.carry = result < value;
    m_flags.halfCarry = (m_registers.a & 0xF) + (value & 0xF) > 0xF;

    m_registers.a = result;
}

void CPU::ADDA_r(RegisterOperand target) {
    switch (target) {
        case RegisterOperand::B: add(m_registers.b); break;
        case RegisterOperand::C: add(m_registers.c); break;
        case RegisterOperand::D: add(m_registers.d); break;
        case RegisterOperand::E: add(m_registers.e); break;
        case RegisterOperand::H: add(m_registers.h); break;
        case RegisterOperand::L: add(m_registers.l); break;
        case RegisterOperand::A: add(m_registers.a); break;
    }
}

void CPU::ADDA_n() {
    add(m_mmu.readByte(m_pc++));
}

void CPU::ADDA_HL() {
    add(m_mmu.readByte(m_registers.getHL()));
}

// Add `value` plus the carry flag to the register A, and set/reset the necessary flags
void CPU::addWithCarry(uint8_t value) {
    add(value + (m_flags.carry ? 1 : 0));
}

void CPU::ADCA_r(RegisterOperand target) {
    switch (target) {
        case RegisterOperand::B: addWithCarry(m_registers.b); break;
        case RegisterOperand::C: addWithCarry(m_registers.c); break;
        case RegisterOperand::D: addWithCarry(m_registers.d); break;
        case RegisterOperand::E: addWithCarry(m_registers.e); break;
        case RegisterOperand::H: addWithCarry(m_registers.h); break;
        case RegisterOperand::L: addWithCarry(m_registers.l); break;
        case RegisterOperand::A: addWithCarry(m_registers.a); break;
    }
}

void CPU::ADCA_n() {
    addWithCarry(m_mmu.readByte(m_pc++));
}

void CPU::ADCA_HL() {
    addWithCarry(m_mmu.readByte(m_registers.getHL()));
}

// Subtract `value` from the register A, set the correct flags,
// and store the result in register A
void CPU::subtract(uint8_t value) {
    uint8_t result = m_registers.a - value;

    m_flags.zero = result == 0;
    m_flags.subtract = true;
    m_flags.carry = result > value;
    m_flags.halfCarry = (m_registers.a & 0x0F) < (value & 0x0F);

    m_registers.a = result;
}

void CPU::SUB_r(RegisterOperand target) {
    switch (target) {
        case RegisterOperand::B: subtract(m_registers.b); break;
        case RegisterOperand::C: subtract(m_registers.c); break;
        case RegisterOperand::D: subtract(m_registers.d); break;
        case RegisterOperand::E: subtract(m_registers.e); break;
        case RegisterOperand::H: subtract(m_registers.h); break;
        case RegisterOperand::L: subtract(m_registers.l); break;
        case RegisterOperand::A: subtract(m_registers.a); break;
    }
}

void CPU::SUB_n() {
    subtract(m_mmu.readByte(m_pc++));
}

void CPU::SUB_HL() {
    subtract(m_mmu.readByte(m_registers.getHL()));
}

void CPU::subtractWithCarry(uint8_t value) {
    subtract(value + (m_flags.carry ? 1 : 0));
}

void CPU::SBCA_r(RegisterOperand target) {
    switch (target) {
        case RegisterOperand::B: subtractWithCarry(m_registers.b); break;
        case RegisterOperand::C: subtractWithCarry(m_registers.c); break;
        case RegisterOperand::D: subtractWithCarry(m_registers.d); break;
        case RegisterOperand::E: subtractWithCarry(m_registers.e); break;
        case RegisterOperand::H: subtractWithCarry(m_registers.h); break;
        case RegisterOperand::L: subtractWithCarry(m_registers.l); break;
        case RegisterOperand::A: subtractWithCarry(m_registers.a); break;
    }
}

void CPU::SBCA_n() {
    subtractWithCarry(m_mmu.readByte(m_pc++));
}

void CPU::SBCA_HL() {
    subtractWithCarry(m_mmu.readByte(m_registers.getHL()));
}

void CPU::bitwiseAnd(uint8_t value) {
    m_registers.a &= value;

    m_flags.zero = m_registers.a == 0;
    m_flags.subtract = false;
    m_flags.carry = false;
    m_flags.halfCarry = true;
}

void CPU::AND_r(RegisterOperand target) {
    switch (target) {
        case RegisterOperand::B: bitwiseAnd(m_registers.b); break;
        case RegisterOperand::C: bitwiseAnd(m_registers.c); break;
        case RegisterOperand::D: bitwiseAnd(m_registers.d); break;
        case RegisterOperand::E: bitwiseAnd(m_registers.e); break;
        case RegisterOperand::H: bitwiseAnd(m_registers.h); break;
        case RegisterOperand::L: bitwiseAnd(m_registers.l); break;
        case RegisterOperand::A: bitwiseAnd(m_registers.a); break;
    }
}

void CPU::AND_n() {
    bitwiseAnd(m_mmu.readByte(m_pc++));
}

void CPU::AND_HL() {
    bitwiseAnd(m_mmu.readByte(m_registers.getHL()));
}

void CPU::bitwiseOr(uint8_t value) {
    m_registers.a |= value;

    m_flags.zero = m_registers.a == 0;
    m_flags.subtract = false;
    m_flags.carry = false;
    m_flags.halfCarry = false;
}

void CPU::OR_r(RegisterOperand target) {
    switch (target) {
        case RegisterOperand::B: bitwiseOr(m_registers.b); break;
        case RegisterOperand::C: bitwiseOr(m_registers.c); break;
        case RegisterOperand::D: bitwiseOr(m_registers.d); break;
        case RegisterOperand::E: bitwiseOr(m_registers.e); break;
        case RegisterOperand::H: bitwiseOr(m_registers.h); break;
        case RegisterOperand::L: bitwiseOr(m_registers.l); break;
        case RegisterOperand::A: bitwiseOr(m_registers.a); break;
    }
}

void CPU::OR_n() {
    bitwiseOr(m_mmu.readByte(m_pc++));
}

void CPU::OR_HL() {
    bitwiseOr(m_mmu.readByte(m_registers.getHL()));
}

void CPU::bitwiseXor(uint8_t value) {
    m_registers.a ^= value;

    m_flags.zero = m_registers.a == 0;
    m_flags.subtract = false;
    m_flags.carry = false;
    m_flags.halfCarry = false;
}

void CPU::XOR_r(RegisterOperand target) {
    switch (target) {
        case RegisterOperand::B: bitwiseXor(m_registers.b); break;
        case RegisterOperand::C: bitwiseXor(m_registers.c); break;
        case RegisterOperand::D: bitwiseXor(m_registers.d); break;
        case RegisterOperand::E: bitwiseXor(m_registers.e); break;
        case RegisterOperand::H: bitwiseXor(m_registers.h); break;
        case RegisterOperand::L: bitwiseXor(m_registers.l); break;
        case RegisterOperand::A: bitwiseXor(m_registers.a); break;
    }
}

void CPU::XOR_n() {
    bitwiseXor(m_mmu.readByte(m_pc++));
}

void CPU::XOR_HL() {
    bitwiseXor(m_mmu.readByte(m_registers.getHL()));
}

// Compares `value` with (subtracts it from) the register A, setting the appropriate
// flags but not storing the result.
void CPU::compare(uint8_t value) {
    uint8_t result = m_registers.a - value;

    m_flags.zero = result == 0;
    m_flags.subtract = true;
    m_flags.carry = result > value;
    m_flags.halfCarry = (m_registers.a & 0x0F) < (value & 0x0F);
}

void CPU::CP_r(RegisterOperand target) {
    switch (target) {
        case RegisterOperand::B: compare(m_registers.b); break;
        case RegisterOperand::C: compare(m_registers.c); break;
        case RegisterOperand::D: compare(m_registers.d); break;
        case RegisterOperand::E: compare(m_registers.e); break;
        case RegisterOperand::H: compare(m_registers.h); break;
        case RegisterOperand::L: compare(m_registers.l); break;
        case RegisterOperand::A: compare(m_registers.a); break;
    }
}

void CPU::CP_n() {
    compare(m_mmu.readByte(m_pc++));
}

void CPU::CP_HL() {
    compare(m_mmu.readByte(m_registers.getHL()));
}

void CPU::increment(uint8_t &target) {
    uint8_t result = target + 1;

    m_flags.zero = result == 0;
    m_flags.subtract = false;
    m_flags.halfCarry = (target & (1 << 2)) != 0 && (result & (1 << 2)) == 0;

    target = result;
}

void CPU::INC_r(RegisterOperand target) {
    switch (target) {
        case RegisterOperand::B: increment(m_registers.b); break;
        case RegisterOperand::C: increment(m_registers.c); break;
        case RegisterOperand::D: increment(m_registers.d); break;
        case RegisterOperand::E: increment(m_registers.e); break;
        case RegisterOperand::H: increment(m_registers.h); break;
        case RegisterOperand::L: increment(m_registers.l); break;
        case RegisterOperand::A: increment(m_registers.a); break;
    }
}

void CPU::INC_HL() {
    uint8_t dummy = m_mmu.readByte(m_registers.getHL());
    increment(dummy);
    m_mmu.writeByte(m_registers.getHL(), dummy);
}

void CPU::decrement(uint8_t& target) {
    uint8_t result = target - 1;

    m_flags.zero = result == 0;
    m_flags.subtract = true;
    m_flags.halfCarry = ((result ^ 0x01 ^ target) & 0x10) == 0x10;

    target = result;
}

void CPU::DEC_r(RegisterOperand target) {
    switch (target) {
        case RegisterOperand::B: decrement(m_registers.b); break;
        case RegisterOperand::C: decrement(m_registers.c); break;
        case RegisterOperand::D: decrement(m_registers.d); break;
        case RegisterOperand::E: decrement(m_registers.e); break;
        case RegisterOperand::H: decrement(m_registers.h); break;
        case RegisterOperand::L: decrement(m_registers.l); break;
        case RegisterOperand::A: decrement(m_registers.a); break;
    }
}

void CPU::DEC_HL() {
    uint8_t dummy = m_mmu.readByte(m_registers.getHL());
    decrement(dummy);
    m_mmu.writeByte(m_registers.getHL(), dummy);
}

// Retroactively transform the previous addition or subtraction into a BCD operation
void CPU::DAA() {
    if (m_flags.subtract) {
        // After a subtraction, only adjust if a carry and/or half carry occurred
        if (m_flags.carry) {
            m_registers.a -= 0x60;
        }

        if (m_flags.halfCarry) {
            m_registers.a -= 0x6;
        }
    } else {
        // After an addition, only adjust if a carry and/or half carry occurred
        // or if the result is out of bounds
        if (m_flags.carry || m_registers.a > 0x99) {
            m_registers.a += 0x60;
            m_flags.carry = true;
        }

        if (m_flags.halfCarry || (m_registers.a & 0x0f) > 0x09) {
            m_registers.a += 0x6;
        }
    }

    // Always update these flags
    m_flags.zero = m_registers.a == 0;
    m_flags.halfCarry = false;
}

// Invert the contents of register A
void CPU::CPL() {
    m_registers.a ^= 0xFF;

    m_flags.subtract = true;
    m_flags.halfCarry = true;
}

// Invert the carry flag
void CPU::CCF() {
    m_flags.carry = !m_flags.carry;

    m_flags.subtract = false;
    m_flags.halfCarry = false;
}

// Set the carry flag
void CPU::SCF() {
    m_flags.carry = true;

    m_flags.subtract = false;
    m_flags.halfCarry = false;
}

void CPU::NOP() {
    // Nothing!
}

// Rotate `target` left 1 bit position, copying the sign bit to the carry flag and bit 0
void CPU::rotateLeft(uint8_t &target) {
    uint8_t signBit = target >> 7u;

    target <<= 1u;
    target ^= signBit;

    m_flags.carry = signBit != 0;
    m_flags.halfCarry = false;
    m_flags.subtract = false;
}

void CPU::RLCA() {
    rotateLeft(m_registers.a);
}

void CPU::RLC_r(RegisterOperand target) {
    switch (target) {
        case RegisterOperand::B: rotateLeft(m_registers.b); break;
        case RegisterOperand::C: rotateLeft(m_registers.c); break;
        case RegisterOperand::D: rotateLeft(m_registers.d); break;
        case RegisterOperand::E: rotateLeft(m_registers.e); break;
        case RegisterOperand::H: rotateLeft(m_registers.h); break;
        case RegisterOperand::L: rotateLeft(m_registers.l); break;
        case RegisterOperand::A: rotateLeft(m_registers.a); break;
    }
}

void CPU::RLC_HL() {
    uint8_t dummy = m_mmu.readByte(m_registers.getHL());
    rotateLeft(dummy);
    m_mmu.writeByte(m_registers.getHL(), dummy);
}

// Rotate `target` left 1 bit position through the carry flag,
// copying the previous contents of the carry flag to bit 0
void CPU::rotateLeftThroughCarry(uint8_t &target) {
    uint8_t prevCarryFlag = (m_flags.carry ? 1 : 0);

    std::bitset<9> value{target};
    value[8] = prevCarryFlag;

    value <<= 1u;
    value[0] = prevCarryFlag;

    target = static_cast<uint8_t>(value.to_ulong());

    m_flags.carry = value[8];
    m_flags.halfCarry = false;
    m_flags.subtract = false;
}

void CPU::RLA() {
    rotateLeftThroughCarry(m_registers.a);
}

void CPU::RL_r(RegisterOperand target) {
    switch (target) {
        case RegisterOperand::B: rotateLeftThroughCarry(m_registers.b); break;
        case RegisterOperand::C: rotateLeftThroughCarry(m_registers.c); break;
        case RegisterOperand::D: rotateLeftThroughCarry(m_registers.d); break;
        case RegisterOperand::E: rotateLeftThroughCarry(m_registers.e); break;
        case RegisterOperand::H: rotateLeftThroughCarry(m_registers.h); break;
        case RegisterOperand::L: rotateLeftThroughCarry(m_registers.l); break;
        case RegisterOperand::A: rotateLeftThroughCarry(m_registers.a); break;
    }
}

void CPU::RL_HL() {
    uint8_t dummy = m_mmu.readByte(m_registers.getHL());
    rotateLeftThroughCarry(dummy);
    m_mmu.writeByte(m_registers.getHL(), dummy);
}

void CPU::rotateRight(uint8_t &target) {
    uint8_t zerothBit = target & 1u;

    target >>= 1u;
    target ^= (zerothBit << 7u);

    m_flags.carry = zerothBit != 0;
    m_flags.halfCarry = false;
    m_flags.subtract = false;
}

void CPU::RRCA() {
    rotateRight(m_registers.a);
}

void CPU::RRC_r(RegisterOperand target) {
    switch (target) {
        case RegisterOperand::B: rotateRight(m_registers.b); break;
        case RegisterOperand::C: rotateRight(m_registers.c); break;
        case RegisterOperand::D: rotateRight(m_registers.d); break;
        case RegisterOperand::E: rotateRight(m_registers.e); break;
        case RegisterOperand::H: rotateRight(m_registers.h); break;
        case RegisterOperand::L: rotateRight(m_registers.l); break;
        case RegisterOperand::A: rotateRight(m_registers.a); break;
    }
}

void CPU::RRC_HL() {
    uint8_t dummy = m_mmu.readByte(m_registers.getHL());
    rotateRight(dummy);
    m_mmu.writeByte(m_registers.getHL(), dummy);
}

void CPU::rotateRightThroughCarry(uint8_t &target) {
    uint8_t prevCarryFlag = (m_flags.carry ? 1 : 0);

    std::bitset<9> value{target};
    value <<= 1u;
    value[0] = prevCarryFlag;

    value >>= 1u;
    value[8] = prevCarryFlag;

    target = static_cast<uint8_t>((value >> 1).to_ulong());

    m_flags.carry = value[0];
    m_flags.halfCarry = false;
    m_flags.subtract = false;
}

void CPU::RRA() {
    rotateRightThroughCarry(m_registers.a);
}

void CPU::RR_r(RegisterOperand target) {
    switch (target) {
        case RegisterOperand::B: rotateRightThroughCarry(m_registers.b); break;
        case RegisterOperand::C: rotateRightThroughCarry(m_registers.c); break;
        case RegisterOperand::D: rotateRightThroughCarry(m_registers.d); break;
        case RegisterOperand::E: rotateRightThroughCarry(m_registers.e); break;
        case RegisterOperand::H: rotateRightThroughCarry(m_registers.h); break;
        case RegisterOperand::L: rotateRightThroughCarry(m_registers.l); break;
        case RegisterOperand::A: rotateRightThroughCarry(m_registers.a); break;
    }
}

void CPU::RR_HL() {
    uint8_t dummy = m_mmu.readByte(m_registers.getHL());
    rotateRightThroughCarry(dummy);
    m_mmu.writeByte(m_registers.getHL(), dummy);
}

// Shift `target` to the left by 1 bit position, after copying bit 7 into the carry flag
void CPU::shiftLeft(uint8_t& target) {
    m_flags.carry = (target >> 7u) != 0;

    target <<= 1u;

    m_flags.zero = target == 0;
    m_flags.halfCarry = false;
    m_flags.subtract = false;
}

void CPU::SLA_r(RegisterOperand target) {
    switch (target) {
        case RegisterOperand::B: shiftLeft(m_registers.b); break;
        case RegisterOperand::C: shiftLeft(m_registers.c); break;
        case RegisterOperand::D: shiftLeft(m_registers.d); break;
        case RegisterOperand::E: shiftLeft(m_registers.e); break;
        case RegisterOperand::H: shiftLeft(m_registers.h); break;
        case RegisterOperand::L: shiftLeft(m_registers.l); break;
        case RegisterOperand::A: shiftLeft(m_registers.a); break;
    }
}

void CPU::SLA_HL() {
    uint8_t dummy = m_mmu.readByte(m_registers.getHL());
    shiftLeft(dummy);
    m_mmu.writeByte(m_registers.getHL(), dummy);
}

// Shift the lower 7 bits (0-6) of `target` 1 bit position to the right,
// after copying bit 0 into the carry flag.
void CPU::shiftTailRight(uint8_t& target) {
    m_flags.carry = (target & 1u) != 0;

    // Save the 7th bit
    uint8_t bit7 = (target >> 7u);

    // Shift the entire number right
    target >>= 1u;

    // Restore the 7th bit
    target |= (bit7 << 7u);

    m_flags.zero = target == 0;
    m_flags.halfCarry = false;
    m_flags.subtract = false;
}

void CPU::SRA_r(RegisterOperand target) {
    switch (target) {
        case RegisterOperand::B: shiftTailRight(m_registers.b); break;
        case RegisterOperand::C: shiftTailRight(m_registers.c); break;
        case RegisterOperand::D: shiftTailRight(m_registers.d); break;
        case RegisterOperand::E: shiftTailRight(m_registers.e); break;
        case RegisterOperand::H: shiftTailRight(m_registers.h); break;
        case RegisterOperand::L: shiftTailRight(m_registers.l); break;
        case RegisterOperand::A: shiftTailRight(m_registers.a); break;
    }
}

void CPU::SRA_HL() {
    uint8_t dummy = m_mmu.readByte(m_registers.getHL());
    shiftTailRight(dummy);
    m_mmu.writeByte(m_registers.getHL(), dummy);
}

// Shift `target` to the right by 1 bit position, after copying bit 0 into the carry flag
void CPU::shiftRight(uint8_t& target) {
    m_flags.carry = (target & 1u) != 0;

    target >>= 1u;

    m_flags.zero = target == 0;
    m_flags.halfCarry = false;
    m_flags.subtract = false;
}

void CPU::SRL_r(RegisterOperand target) {
    switch (target) {
        case RegisterOperand::B: shiftRight(m_registers.b); break;
        case RegisterOperand::C: shiftRight(m_registers.c); break;
        case RegisterOperand::D: shiftRight(m_registers.d); break;
        case RegisterOperand::E: shiftRight(m_registers.e); break;
        case RegisterOperand::H: shiftRight(m_registers.h); break;
        case RegisterOperand::L: shiftRight(m_registers.l); break;
        case RegisterOperand::A: shiftRight(m_registers.a); break;
    }
}

void CPU::SRL_HL() {
    uint8_t dummy = m_mmu.readByte(m_registers.getHL());
    shiftRight(dummy);
    m_mmu.writeByte(m_registers.getHL(), dummy);
}

// Check if the bit at position `bit` in `byte` is zero. Set the zero flag accordingly.
void CPU::testBit(BitOperand bit, uint8_t byte) {
    uint8_t nthBit = ((byte >> static_cast<uint8_t>(bit)) & 1u);

    m_flags.zero = nthBit == 0;
    m_flags.halfCarry = true;
    m_flags.subtract = false;
}

void CPU::BIT_b_r(BitOperand bit, RegisterOperand reg) {
    switch (reg) {
        case RegisterOperand::B: testBit(bit, m_registers.b); break;
        case RegisterOperand::C: testBit(bit, m_registers.c); break;
        case RegisterOperand::D: testBit(bit, m_registers.d); break;
        case RegisterOperand::E: testBit(bit, m_registers.e); break;
        case RegisterOperand::H: testBit(bit, m_registers.h); break;
        case RegisterOperand::L: testBit(bit, m_registers.l); break;
        case RegisterOperand::A: testBit(bit, m_registers.a); break;
    }
}

void CPU::BIT_b_HL(BitOperand bit) {
    testBit(bit, m_mmu.readByte(m_registers.getHL()));
}

// Set the bit at position `bit` in `target`. No flags are affected.
void CPU::setBit(BitOperand bit, uint8_t &target) {
    target |= (1u << static_cast<uint8_t>(bit));
}

void CPU::SET_b_r(BitOperand bit, RegisterOperand reg) {
    switch (reg) {
        case RegisterOperand::B: setBit(bit, m_registers.b); break;
        case RegisterOperand::C: setBit(bit, m_registers.c); break;
        case RegisterOperand::D: setBit(bit, m_registers.d); break;
        case RegisterOperand::E: setBit(bit, m_registers.e); break;
        case RegisterOperand::H: setBit(bit, m_registers.h); break;
        case RegisterOperand::L: setBit(bit, m_registers.l); break;
        case RegisterOperand::A: setBit(bit, m_registers.a); break;
    }
}

void CPU::SET_b_HL(BitOperand bit) {
    uint8_t dummy = m_mmu.readByte(m_registers.getHL());
    setBit(bit, dummy);
    m_mmu.writeByte(m_registers.getHL(), dummy);
}

// Reset the bit at position `bit` in `target`. No flags are affected.
void CPU::resetBit(BitOperand bit, uint8_t &target) {
    target &= ~(1u << static_cast<uint8_t>(bit));
}

void CPU::RES_b_r(BitOperand bit, RegisterOperand reg) {
    switch (reg) {
        case RegisterOperand::B: resetBit(bit, m_registers.b); break;
        case RegisterOperand::C: resetBit(bit, m_registers.c); break;
        case RegisterOperand::D: resetBit(bit, m_registers.d); break;
        case RegisterOperand::E: resetBit(bit, m_registers.e); break;
        case RegisterOperand::H: resetBit(bit, m_registers.h); break;
        case RegisterOperand::L: resetBit(bit, m_registers.l); break;
        case RegisterOperand::A: resetBit(bit, m_registers.a); break;
    }
}

void CPU::RES_b_HL(BitOperand bit) {
    uint8_t dummy = m_mmu.readByte(m_registers.getHL());
    resetBit(bit, dummy);
    m_mmu.writeByte(m_registers.getHL(), dummy);
}