#ifndef BIGBOY_CPU_H
#define BIGBOY_CPU_H

#include <bigboy/MMU.h>
#include <bigboy/OpCode.h>
#include <bigboy/PrefixOpCode.h>
#include <bigboy/Registers.h>

enum class BitOperand : uint8_t {
    BIT0 = 0, // 000
    BIT1 = 1, // 001
    BIT2 = 2, // 010
    BIT3 = 3, // 011
    BIT4 = 4, // 100
    BIT5 = 5, // 101
    BIT6 = 6, // 110
    BIT7 = 7  // 111
};

enum class ResetOperand : uint16_t {
    x00 = 0x00,
    x08 = 0x08,
    x10 = 0x10,
    x18 = 0x18,
    x20 = 0x20,
    x28 = 0x28,
    x30 = 0x30,
    x38 = 0x38
};

enum class Interrupt : uint8_t {
    VBLANK = 0,
    LCD_STAT = 1,
    TIMER = 2,
    SERIAL = 3,
    JOYPAD = 4
};

constexpr uint8_t INTERRUPT_COUNT = 5;

class CPU {
public:
    explicit CPU(MMU& mmu);
    void reset();

    uint8_t step();

    void requestInterrupt(Interrupt interrupt);
    void handleInterrupts();

private:
    uint8_t stepPrefix();

    uint8_t nextByte();
    uint16_t nextWord();

    std::string disassembleCurrent();

    void serviceInterrupt(Interrupt interrupt);

    bool isInterruptServicable(Interrupt interrupt) const;
    bool isInterruptEnabled(Interrupt interrupt) const;
    bool isInterruptRequested(Interrupt interrupt) const;

    void enableInterrupt(Interrupt interrupt);
    void disableInterrupt(Interrupt interrupt);

    void unrequestInterrupt(Interrupt interrupt);

    void load(uint8_t& target, uint8_t value);

    uint8_t LD_r_r(RegisterOperand target, RegisterOperand value);
    uint8_t LD_r_n(RegisterOperand target);
    uint8_t LD_r_HL(RegisterOperand target);

    uint8_t LD_HL_r(RegisterOperand value);
    uint8_t LD_HL_n();

    uint8_t LD_A_BC();
    uint8_t LD_A_DE();
    uint8_t LD_A_nn();

    uint8_t LD_BC_A();
    uint8_t LD_DE_A();
    uint8_t LD_nn_A();

    uint8_t LD_A_FF00n();
    uint8_t LD_FF00n_A();

    uint8_t LD_A_FF00C();
    uint8_t LD_FF00C_A();

    uint8_t LDI_HL_A();
    uint8_t LDI_A_HL();

    uint8_t LDD_HL_A();
    uint8_t LDD_A_HL();

    void load(uint16_t& target, uint16_t value);

    uint8_t LD_dd_nn(RegisterPairOperand target);
    uint8_t LD_nn_SP();
    uint8_t LD_SP_HL();

    void push(uint16_t value);

    uint8_t PUSH_qq(RegisterPairStackOperand value);

    void pop(uint16_t& target);

    uint8_t POP_qq(RegisterPairStackOperand target);

    void add(uint8_t value);

    uint8_t ADDA_r(RegisterOperand target);
    uint8_t ADDA_n();
    uint8_t ADDA_HL();

    void addWithCarry(uint8_t value);

    uint8_t ADCA_r(RegisterOperand target);
    uint8_t ADCA_n();
    uint8_t ADCA_HL();

    void subtract(uint8_t value);

    uint8_t SUB_r(RegisterOperand target);
    uint8_t SUB_n();
    uint8_t SUB_HL();

    void subtractWithCarry(uint8_t value);

    uint8_t SBCA_r(RegisterOperand target);
    uint8_t SBCA_n();
    uint8_t SBCA_HL();

    void bitwiseAnd(uint8_t value);

    uint8_t AND_r(RegisterOperand target);
    uint8_t AND_n();
    uint8_t AND_HL();

    void bitwiseXor(uint8_t value);

    uint8_t XOR_r(RegisterOperand target);
    uint8_t XOR_n();
    uint8_t XOR_HL();

    void bitwiseOr(uint8_t value);

    uint8_t OR_r(RegisterOperand target);
    uint8_t OR_n();
    uint8_t OR_HL();

    void compare(uint8_t value);

    uint8_t CP_r(RegisterOperand target);
    uint8_t CP_n();
    uint8_t CP_HL();

    void increment(uint8_t &target);

    uint8_t INC_r(RegisterOperand target);
    uint8_t INC_HL();

    void decrement(uint8_t &target);

    uint8_t DEC_r(RegisterOperand target);
    uint8_t DEC_HL_();

    uint8_t DAA();
    uint8_t CPL();

    void add(uint16_t& target, uint16_t value);

    uint8_t ADD_HL_rr(RegisterPairOperand value);

    void increment(uint16_t& target);

    uint8_t INC_rr(RegisterPairOperand target);

    void decrement(uint16_t& target);

    uint8_t DEC_rr(RegisterPairOperand target);

    void add(uint16_t& target, int8_t value);

    uint8_t ADD_SP_s();
    uint8_t LD_HL_SPs();

    void rotateLeft(uint8_t &target);

    uint8_t RLCA();
    uint8_t RLC_r(RegisterOperand target);
    uint8_t RLC_HL();

    void rotateLeftThroughCarry(uint8_t &target);

    uint8_t RLA();
    uint8_t RL_r(RegisterOperand target);
    uint8_t RL_HL();

    void rotateRight(uint8_t &target);

    uint8_t RRCA();
    uint8_t RRC_r(RegisterOperand target);
    uint8_t RRC_HL();

    void rotateRightThroughCarry(uint8_t &target);

    uint8_t RRA();
    uint8_t RR_r(RegisterOperand target);
    uint8_t RR_HL();

    void shiftLeft(uint8_t &target);

    uint8_t SLA_r(RegisterOperand target);
    uint8_t SLA_HL();

    void swap(uint8_t& target);

    uint8_t SWAP_r(RegisterOperand target);
    uint8_t SWAP_HL();

    void shiftTailRight(uint8_t &target);

    uint8_t SRA_r(RegisterOperand target);
    uint8_t SRA_HL();

    void shiftRight(uint8_t &target);

    uint8_t SRL_r(RegisterOperand target);
    uint8_t SRL_HL();

    void testBit(BitOperand bit, uint8_t byte);

    uint8_t BIT_b_r(BitOperand bit, RegisterOperand reg);
    uint8_t BIT_b_HL(BitOperand bit);

    void setBit(BitOperand bit, uint8_t& target);

    uint8_t SET_b_r(BitOperand bit, RegisterOperand reg);
    uint8_t SET_b_HL(BitOperand bit);

    void resetBit(BitOperand bit, uint8_t& target);

    uint8_t RES_b_r(BitOperand bit, RegisterOperand reg);
    uint8_t RES_b_HL(BitOperand bit);

    uint8_t CCF();
    uint8_t SCF();

    uint8_t NOP();

    uint8_t HALT();
    uint8_t STOP();

    uint8_t DI();
    uint8_t EI();

    void absoluteJump(uint16_t address);

    uint8_t JP_nn();
    uint8_t JP_HL();
    uint8_t JP_f_nn(ConditionOperand condition);

    void relativeJump(int8_t offset);

    uint8_t JR_PCdd();
    uint8_t JR_f_PCdd(ConditionOperand condition);

    void call(uint16_t address);

    uint8_t CALL_nn();
    uint8_t CALL_f_nn(ConditionOperand condition);

    void ret();

    uint8_t RET();
    uint8_t RET_f(ConditionOperand condition);
    uint8_t RETI();

    uint8_t RST(ResetOperand address);

    MMU& m_mmu;

    Registers m_registers{};

    // Total number of cycles since execution began
    uint64_t m_clock;

    uint16_t m_pc; // Program counter.

    // Are we halted? HALT will set this, and interrupts will reset this.
    bool m_halted;

    // Are we stopped? STOP will set this.
    bool m_stopped;

    // Interrupt master enable flag. EI/DI will set/reset this.
    bool m_ime;
};

#endif //BIGBOY_CPU_H
