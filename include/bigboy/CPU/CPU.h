#ifndef BIGBOY_CPU_H
#define BIGBOY_CPU_H

#include <bigboy/CPU/MMU.h>
#include <bigboy/CPU/OpCode.h>
#include <bigboy/CPU/PrefixOpCode.h>
#include <bigboy/CPU/Registers.h>

class CPU {
public:
    struct Clock {
        int m;
        int t;
    };

private:
    Clock m_clock{};
    Registers m_registers{};
    Flags m_flags{m_registers.f};

    MMU m_mmu{};

    uint16_t m_pc = 0;
    uint8_t m_halt = 0;
    uint8_t m_stop = 0;

    void load(uint8_t& target, uint8_t value);

    void LD_r_r(RegisterOperand target, RegisterOperand value);
    void LD_r_n(RegisterOperand target);
    void LD_r_HL(RegisterOperand target);

    void LD_HL_r(RegisterOperand value);
    void LD_HL_n();

    void LD_A_BC();
    void LD_A_DE();
    void LD_A_nn();

    void LD_BC_A();
    void LD_DE_A();
    void LD_nn_A();

    void load(uint16_t& target, uint16_t value);

    void LD_dd_nn(RegisterPairOperand target);
    void LD_HL_nn();
    void LD_nn_HL();
    void LD_SP_HL();

    void push(uint16_t value);

    void PUSH_qq(RegisterPairStackOperand value);

    void pop(uint16_t& target);

    void POP_qq(RegisterPairStackOperand target);

    void add(uint8_t value);

    void ADDA_r(RegisterOperand target);
    void ADDA_n();
    void ADDA_HL();

    void addWithCarry(uint8_t value);

    void ADCA_r(RegisterOperand target);
    void ADCA_n();
    void ADCA_HL();

    void subtract(uint8_t value);

    void SUB_r(RegisterOperand target);
    void SUB_n();
    void SUB_HL();

    void subtractWithCarry(uint8_t value);

    void SBCA_r(RegisterOperand target);
    void SBCA_n();
    void SBCA_HL();

    void bitwiseAnd(uint8_t value);

    void AND_r(RegisterOperand target);
    void AND_n();
    void AND_HL();

    void bitwiseOr(uint8_t value);

    void OR_r(RegisterOperand target);
    void OR_n();
    void OR_HL();

    void bitwiseXor(uint8_t value);

    void XOR_r(RegisterOperand target);
    void XOR_n();
    void XOR_HL();

    void compare(uint8_t value);

    void CP_r(RegisterOperand target);
    void CP_n();
    void CP_HL();

    void increment(uint8_t &target);

    void INC_r(RegisterOperand target);
    void INC_HL();

    void decrement(uint8_t &target);

    void DEC_r(RegisterOperand target);
    void DEC_HL();

    void DAA();
    void CPL();

    void CCF();
    void SCF();

    void NOP();
    void HALT();

    void rotateLeft(uint8_t &target);

    void RLCA();
    void RLC_r(RegisterOperand target);
    void RLC_HL();

    void rotateLeftThroughCarry(uint8_t &target);

    void RLA();
    void RL_r(RegisterOperand target);
    void RL_HL();

    void rotateRight(uint8_t &target);

    void RRCA();
    void RRC_r(RegisterOperand target);
    void RRC_HL();

    void rotateRightThroughCarry(uint8_t &target);

    void RRA();
    void RR_r(RegisterOperand target);
    void RR_HL();

    void shiftLeft(uint8_t &target);

    void SLA_r(RegisterOperand target);
    void SLA_HL();

    void shiftTailRight(uint8_t &target);

    void SRA_r(RegisterOperand target);
    void SRA_HL();

    void shiftRight(uint8_t &target);

    void SRL_r(RegisterOperand target);
    void SRL_HL();

    void testBit(BitOperand bit, uint8_t byte);

    void BIT_b_r(BitOperand bit, RegisterOperand reg);
    void BIT_b_HL(BitOperand bit);

    void setBit(BitOperand bit, uint8_t& target);

    void SET_b_r(BitOperand bit, RegisterOperand reg);
    void SET_b_HL(BitOperand bit);

    void resetBit(BitOperand bit, uint8_t& target);

    void RES_b_r(BitOperand bit, RegisterOperand reg);
    void RES_b_HL(BitOperand bit);

public:
    void load(const std::array<uint8_t, 0xFFFF> &memory);

    void exec();

    void step();
    void stepPrefix();

    void reset();

    Registers& registers();
    const Registers& registers() const;
};

#endif //BIGBOY_CPU_H
