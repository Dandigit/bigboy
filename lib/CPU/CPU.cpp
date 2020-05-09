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

void CPU::load(uint8_t& target, uint8_t value) {
    target = value;
}

void CPU::LD_r_r(RegisterOperand target, RegisterOperand value) {
    load(m_registers.get(target), m_registers.get(value));
}

void CPU::LD_r_n(RegisterOperand target) {
    load(m_registers.get(target), m_mmu.byteAt(m_pc++));
}

void CPU::LD_r_HL(RegisterOperand target) {
    load(m_registers.get(target), m_mmu.byteAt(m_registers.HL()));
}

void CPU::LD_HL_r(RegisterOperand value) {
    load(m_mmu.byteAt(m_registers.HL()), m_registers.get(value));
}

void CPU::LD_HL_n() {
    load(m_mmu.byteAt(m_registers.HL()), m_mmu.byteAt(m_pc++));
}

void CPU::LD_A_BC() {
    load(m_registers.a, m_mmu.byteAt(m_registers.BC()));
}

void CPU::LD_A_DE() {
    load(m_registers.a, m_mmu.byteAt(m_registers.DE()));
}

void CPU::LD_A_nn() {
    uint8_t lower = m_mmu.byteAt(m_pc++);
    uint8_t higher = m_mmu.byteAt(m_pc++);
    uint16_t nn = (higher << 8u) | lower;

    load(m_registers.a, m_mmu.byteAt(nn));
}

void CPU::LD_BC_A() {
    load(m_mmu.byteAt(m_registers.BC()), m_registers.a);
}

void CPU::LD_DE_A() {
    load(m_mmu.byteAt(m_registers.DE()), m_registers.a);
}

void CPU::LD_nn_A() {
    uint8_t lower = m_mmu.byteAt(m_pc++);
    uint8_t higher = m_mmu.byteAt(m_pc++);
    uint16_t nn = (higher << 8u) | lower;

    load(m_mmu.byteAt(nn), m_registers.a);
}

void CPU::LD_A_FF00n() {
    load(m_registers.a, m_mmu.byteAt(0xFF00 + m_pc++));
}

void CPU::LD_FF00n_A() {
    load(m_mmu.byteAt(0xFF00 + m_pc++), m_registers.a);
}

void CPU::LD_A_FF00C() {
    load(m_registers.a, m_mmu.byteAt(0xFF00 + m_registers.c));
}

void CPU::LD_FF00C_A() {
    load(m_mmu.byteAt(0xFF00 + m_registers.c), m_registers.a);
}

void CPU::LDI_HL_A() {
    load(m_mmu.byteAt(m_registers.HL()), m_registers.a);
    ++m_registers.HL();
}

void CPU::LDI_A_HL() {
    load(m_registers.a, m_mmu.byteAt(m_registers.HL()));
    ++m_registers.HL();
}

void CPU::LDD_HL_A() {
    load(m_mmu.byteAt(m_registers.HL()), m_registers.a);
    --m_registers.HL();
}

void CPU::LDD_A_HL() {
    load(m_registers.a, m_mmu.byteAt(m_registers.HL()));
    --m_registers.HL();
}

void CPU::load(uint16_t& target, uint16_t value) {
    target = value;
}

void CPU::LD_dd_nn(RegisterPairOperand target) {
    uint8_t lower = m_mmu.byteAt(m_pc++);
    uint8_t higher = m_mmu.byteAt(m_pc++);
    uint16_t nn = (higher << 8u) | lower;

    load(m_registers.get(target), nn);
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
    add(m_registers.get(target));
}

void CPU::ADDA_n() {
    add(m_mmu.byteAt(m_pc++));
}

void CPU::ADDA_HL() {
    add(m_mmu.byteAt(m_registers.HL()));
}

// Add `value` plus the carry flag to the register A, and set/reset the necessary flags
void CPU::addWithCarry(uint8_t value) {
    add(value + (m_flags.carry ? 1 : 0));
}

void CPU::ADCA_r(RegisterOperand target) {
    addWithCarry(m_registers.get(target));
}

void CPU::ADCA_n() {
    addWithCarry(m_mmu.byteAt(m_pc++));
}

void CPU::ADCA_HL() {
    addWithCarry(m_mmu.byteAt(m_registers.HL()));
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
    subtract(m_registers.get(target));
}

void CPU::SUB_n() {
    subtract(m_mmu.byteAt(m_pc++));
}

void CPU::SUB_HL() {
    subtract(m_mmu.byteAt(m_registers.HL()));
}

void CPU::subtractWithCarry(uint8_t value) {
    subtract(value + (m_flags.carry ? 1 : 0));
}

void CPU::SBCA_r(RegisterOperand target) {
    subtractWithCarry(m_registers.get(target));
}

void CPU::SBCA_n() {
    subtractWithCarry(m_mmu.byteAt(m_pc++));
}

void CPU::SBCA_HL() {
    subtractWithCarry(m_mmu.byteAt(m_registers.HL()));
}

void CPU::bitwiseAnd(uint8_t value) {
    m_registers.a &= value;

    m_flags.zero = m_registers.a == 0;
    m_flags.subtract = false;
    m_flags.carry = false;
    m_flags.halfCarry = true;
}

void CPU::AND_r(RegisterOperand target) {
    bitwiseAnd(m_registers.get(target));
}

void CPU::AND_n() {
    bitwiseAnd(m_mmu.byteAt(m_pc++));
}

void CPU::AND_HL() {
    bitwiseAnd(m_mmu.byteAt(m_registers.HL()));
}

void CPU::bitwiseXor(uint8_t value) {
    m_registers.a ^= value;

    m_flags.zero = m_registers.a == 0;
    m_flags.subtract = false;
    m_flags.carry = false;
    m_flags.halfCarry = false;
}

void CPU::XOR_r(RegisterOperand target) {
    bitwiseXor(m_registers.get(target));
}

void CPU::XOR_n() {
    bitwiseXor(m_mmu.byteAt(m_pc++));
}

void CPU::XOR_HL() {
    bitwiseXor(m_mmu.byteAt(m_registers.HL()));
}

void CPU::bitwiseOr(uint8_t value) {
    m_registers.a |= value;

    m_flags.zero = m_registers.a == 0;
    m_flags.subtract = false;
    m_flags.carry = false;
    m_flags.halfCarry = false;
}

void CPU::OR_r(RegisterOperand target) {
    bitwiseOr(m_registers.get(target));
}

void CPU::OR_n() {
    bitwiseOr(m_mmu.byteAt(m_pc++));
}

void CPU::OR_HL() {
    bitwiseOr(m_mmu.byteAt(m_registers.HL()));
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
    compare(m_registers.get(target));
}

void CPU::CP_n() {
    compare(m_mmu.byteAt(m_pc++));
}

void CPU::CP_HL() {
    compare(m_mmu.byteAt(m_registers.HL()));
}

void CPU::increment(uint8_t &target) {
    uint8_t result = target + 1;

    m_flags.zero = result == 0;
    m_flags.subtract = false;
    m_flags.halfCarry = (target & (1 << 2)) != 0 && (result & (1 << 2)) == 0;

    target = result;
}

void CPU::INC_r(RegisterOperand target) {
    increment(m_registers.get(target));
}

void CPU::INC_HL() {
    increment(m_mmu.byteAt(m_registers.HL()));
}

void CPU::decrement(uint8_t& target) {
    uint8_t result = target - 1;

    m_flags.zero = result == 0;
    m_flags.subtract = true;
    m_flags.halfCarry = ((result ^ 0x01 ^ target) & 0x10) == 0x10;

    target = result;
}

void CPU::DEC_r(RegisterOperand target) {
    decrement(m_registers.get(target));
}

void CPU::DEC_HL_() {
    decrement(m_mmu.byteAt(m_registers.HL()));
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

// Add `value` to `target`, storing the result in `target` and setting the appropriate flags
void CPU::add(uint16_t &target, uint16_t value) {
    uint16_t result = target + value;

    m_flags.subtract = false;
    m_flags.carry = result < target;
    m_flags.halfCarry = ((result ^ target ^ value) & 0x1000) != 0;

    target = result;
}

void CPU::ADD_HL_rr(RegisterPairOperand value) {
    add(m_registers.HL(), m_registers.get(value));
}

void CPU::increment(uint16_t& target) {
    ++target;
}

void CPU::INC_rr(RegisterPairOperand target) {
    increment(m_registers.get(target));
}

void CPU::decrement(uint16_t& target) {
    --target;
}

void CPU::DEC_rr(RegisterPairOperand target) {
    decrement(m_registers.get(target));
}

void CPU::add(uint16_t &target, int8_t value) {
    uint16_t result = target + value;

    m_flags.zero = false;
    m_flags.subtract = false;
    m_flags.halfCarry = (result & 0xF) < (target & 0xF);
    m_flags.carry = (result & 0xFF) < (target & 0xFF);

    target = result;
}

void CPU::ADD_SP_s() {
    auto s = static_cast<int8_t>(m_mmu.byteAt(m_pc++));
    add(m_registers.sp, s);
}

void CPU::LD_HL_SPs() {
    uint16_t value = m_registers.sp;
    auto s = static_cast<int8_t>(m_mmu.byteAt(m_pc++));
    add(value, s);

    load(m_registers.HL(), value);
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
    rotateLeft(m_registers.get(target));
}

void CPU::RLC_HL() {
    rotateLeft(m_mmu.byteAt(m_registers.HL()));
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
    rotateLeftThroughCarry(m_registers.get(target));
}

void CPU::RL_HL() {
    rotateLeftThroughCarry(m_mmu.byteAt(m_registers.HL()));
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
    rotateRight(m_registers.get(target));
}

void CPU::RRC_HL() {
    rotateRight(m_mmu.byteAt(m_registers.HL()));
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
    rotateRightThroughCarry(m_registers.get(target));
}

void CPU::RR_HL() {
    rotateRightThroughCarry(m_mmu.byteAt(m_registers.HL()));
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
    shiftLeft(m_registers.get(target));
}

void CPU::SLA_HL() {
    shiftLeft(m_mmu.byteAt(m_registers.HL()));
}

// Swap the lower and upper nibbles of `target`, setting the appropriate
// flags.
void CPU::swap(uint8_t& target) {
    target = ((target & 0x0Fu) << 4u | (target & 0xF0u) >> 4u);

    m_flags.zero = target == 0;
    m_flags.subtract = false;
    m_flags.halfCarry = false;
    m_flags.carry = false;
}

void CPU::SWAP_r(RegisterOperand target) {
    swap(m_registers.get(target));
}

void CPU::SWAP_HL() {
    swap(m_mmu.byteAt(m_registers.HL()));
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
    shiftTailRight(m_registers.get(target));
}

void CPU::SRA_HL() {
    shiftTailRight(m_mmu.byteAt(m_registers.HL()));
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
    shiftRight(m_registers.get(target));
}

void CPU::SRL_HL() {
    shiftRight(m_mmu.byteAt(m_registers.HL()));
}

// Check if the bit at position `bit` in `byte` is zero. Set the zero flag accordingly.
void CPU::testBit(BitOperand bit, uint8_t byte) {
    uint8_t nthBit = ((byte >> static_cast<uint8_t>(bit)) & 1u);

    m_flags.zero = nthBit == 0;
    m_flags.halfCarry = true;
    m_flags.subtract = false;
}

void CPU::BIT_b_r(BitOperand bit, RegisterOperand reg) {
    testBit(bit, m_registers.get(reg));
}

void CPU::BIT_b_HL(BitOperand bit) {
    testBit(bit, m_mmu.byteAt(m_registers.HL()));
}

// Set the bit at position `bit` in `target`. No flags are affected.
void CPU::setBit(BitOperand bit, uint8_t &target) {
    target |= (1u << static_cast<uint8_t>(bit));
}

void CPU::SET_b_r(BitOperand bit, RegisterOperand reg) {
    setBit(bit, m_registers.get(reg));
}

void CPU::SET_b_HL(BitOperand bit) {
    setBit(bit, m_mmu.byteAt(m_registers.HL()));
}

// Reset the bit at position `bit` in `target`. No flags are affected.
void CPU::resetBit(BitOperand bit, uint8_t &target) {
    target &= ~(1u << static_cast<uint8_t>(bit));
}

void CPU::RES_b_r(BitOperand bit, RegisterOperand reg) {
    resetBit(bit, m_registers.get(reg));
}

void CPU::RES_b_HL(BitOperand bit) {
    resetBit(bit, m_mmu.byteAt(m_registers.HL()));
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

void CPU::HALT() {
    m_halted = true;
}

void CPU::STOP() {
    m_stopped = true;
}

void CPU::DI() {
    m_ime = false;
}

void CPU::EI() {
    m_ime = true;
}

void CPU::absoluteJump(uint16_t address) {
    m_pc = address;
}

void CPU::JP_nn() {
    uint8_t lower = m_mmu.byteAt(m_pc++);
    uint8_t higher = m_mmu.byteAt(m_pc++);
    uint16_t nn = (higher << 8u) | lower;

    absoluteJump(nn);
}

void CPU::JP_HL() {
    absoluteJump(m_registers.HL());
}

void CPU::JP_f_nn(ConditionOperand condition) {
    uint8_t lower = m_mmu.byteAt(m_pc++);
    uint8_t higher = m_mmu.byteAt(m_pc++);
    uint16_t nn = (higher << 8u) | lower;

    if (m_flags.get(condition)) {
        absoluteJump(nn);
    }
}

void CPU::relativeJump(int8_t offset) {
    m_pc += offset;
}

void CPU::JR_PCdd() {
    auto dd = static_cast<int8_t>(m_mmu.byteAt(m_pc++));
    relativeJump(dd);
}

void CPU::JR_f_PCdd(ConditionOperand condition) {
    auto dd = static_cast<int8_t>(m_mmu.byteAt(m_pc++));
    if (m_flags.get(condition)) {
        relativeJump(dd);
    }
}

void CPU::call(uint16_t address) {
    push(m_pc);
    load(m_pc, address);
}

void CPU::CALL_nn() {
    uint8_t lower = m_mmu.byteAt(m_pc++);
    uint8_t higher = m_mmu.byteAt(m_pc++);
    uint16_t nn = (higher << 8u) | lower;

    call(nn);
}

void CPU::CALL_f_nn(ConditionOperand condition) {
    uint8_t lower = m_mmu.byteAt(m_pc++);
    uint8_t higher = m_mmu.byteAt(m_pc++);
    uint16_t nn = (higher << 8u) | lower;

    if (m_flags.get(condition)) {
        call(nn);
    }
}

void CPU::ret() {
    pop(m_pc);
}

void CPU::RET() {
    ret();
}

void CPU::RET_f(ConditionOperand condition) {
    if (m_flags.get(condition)) {
        ret();
    }
}

void CPU::RETI() {
    ret();
    m_ime = true;
}

void CPU::RST(ResetOperand address) {
    call(static_cast<uint16_t>(address));
}

void CPU::step() {
    if (m_stopped) return;
    if (m_halted) {
        // We need to keep the clock going.
        NOP();
        return;
    }

    auto current = static_cast<OpCode>(m_mmu.byteAt(m_pc++));
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
        case OpCode::LD_A_FF00n:
            LD_A_FF00n();
            break;
        case OpCode::LD_FF00n_A:
            LD_FF00n_A();
            break;
        case OpCode::LD_A_FF00C:
            LD_A_FF00C();
            break;
        case OpCode::LD_FF00C_A:
            LD_FF00C_A();
            break;
        case OpCode::LDI_HL_A:
            LDI_HL_A();
            break;
        case OpCode::LDI_A_HL:
            LDI_A_HL();
            break;
        case OpCode::LDD_HL_A:
            LDD_HL_A();
            break;
        case OpCode::LDD_A_HL:
            LDD_A_HL();
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
        case OpCode::INC_HL_:
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
        case OpCode::DEC_HL_:
            DEC_HL_();
            break;
        case OpCode::DAA:
            DAA();
            break;
        case OpCode::CPL:
            CPL();
            break;
        case OpCode::ADD_HL_BC:
            ADD_HL_rr(RegisterPairOperand::BC);
            break;
        case OpCode::ADD_HL_DE:
            ADD_HL_rr(RegisterPairOperand::DE);
            break;
        case OpCode::ADD_HL_HL:
            ADD_HL_rr(RegisterPairOperand::HL);
            break;
        case OpCode::ADD_HL_SP:
            ADD_HL_rr(RegisterPairOperand::SP);
            break;
        case OpCode::INC_BC:
            INC_rr(RegisterPairOperand::BC);
            break;
        case OpCode::INC_DE:
            INC_rr(RegisterPairOperand::DE);
            break;
        case OpCode::INC_HL:
            INC_rr(RegisterPairOperand::HL);
            break;
        case OpCode::INC_SP:
            INC_rr(RegisterPairOperand::SP);
            break;
        case OpCode::DEC_BC:
            DEC_rr(RegisterPairOperand::BC);
            break;
        case OpCode::DEC_DE:
            DEC_rr(RegisterPairOperand::DE);
            break;
        case OpCode::DEC_HL:
            DEC_rr(RegisterPairOperand::HL);
            break;
        case OpCode::DEC_SP:
            DEC_rr(RegisterPairOperand::SP);
            break;
        case OpCode::ADD_SP_s:
            ADD_SP_s();
            break;
        case OpCode::LD_HL_SPs:
            LD_HL_SPs();
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
        case OpCode::CCF:
            CCF();
            break;
        case OpCode::SCF:
            SCF();
            break;
        case OpCode::NOP:
            NOP();
            break;
        case OpCode::HALT:
            HALT();
            break;
        case OpCode::STOP:
            STOP();
            break;
        case OpCode::DI:
            DI();
            break;
        case OpCode::EI:
            EI();
            break;
        case OpCode::JP_nn:
            JP_nn();
            break;
        case OpCode::JP_HL:
            JP_HL();
            break;
        case OpCode::JP_NZ_nn:
            JP_f_nn(ConditionOperand::NZ);
            break;
        case OpCode::JP_Z_nn:
            JP_f_nn(ConditionOperand::Z);
            break;
        case OpCode::JP_NC_nn:
            JP_f_nn(ConditionOperand::NC);
            break;
        case OpCode::JP_C_nn:
            JP_f_nn(ConditionOperand::C);
            break;
        case OpCode::JR_PCdd:
            JR_PCdd();
            break;
        case OpCode::JR_NZ_PCdd:
            JR_f_PCdd(ConditionOperand::NZ);
            break;
        case OpCode::JR_Z_PCdd:
            JR_f_PCdd(ConditionOperand::Z);
            break;
        case OpCode::JR_NC_PCdd:
            JR_f_PCdd(ConditionOperand::NC);
            break;
        case OpCode::JR_C_PCdd:
            JR_f_PCdd(ConditionOperand::C);
            break;
        case OpCode::CALL_nn:
            CALL_nn();
            break;
        case OpCode::CALL_NZ_nn:
            CALL_f_nn(ConditionOperand::NZ);
            break;
        case OpCode::CALL_Z_nn:
            CALL_f_nn(ConditionOperand::Z);
            break;
        case OpCode::CALL_NC_nn:
            CALL_f_nn(ConditionOperand::NC);
            break;
        case OpCode::CALL_C_nn:
            CALL_f_nn(ConditionOperand::C);
            break;
        case OpCode::RET:
            RET();
            break;
        case OpCode::RET_NZ:
            RET_f(ConditionOperand::NZ);
            break;
        case OpCode::RET_Z:
            RET_f(ConditionOperand::Z);
            break;
        case OpCode::RET_NC:
            RET_f(ConditionOperand::NC);
            break;
        case OpCode::RET_C:
            RET_f(ConditionOperand::C);
            break;
        case OpCode::RETI:
            RETI();
            break;
        case OpCode::RST_00:
            RST(ResetOperand::x00);
            break;
        case OpCode::RST_08:
            RST(ResetOperand::x08);
            break;
        case OpCode::RST_10:
            RST(ResetOperand::x10);
            break;
        case OpCode::RST_18:
            RST(ResetOperand::x18);
            break;
        case OpCode::RST_20:
            RST(ResetOperand::x20);
            break;
        case OpCode::RST_28:
            RST(ResetOperand::x28);
            break;
        case OpCode::RST_30:
            RST(ResetOperand::x30);
            break;
        case OpCode::RST_38:
            RST(ResetOperand::x38);
            break;
        case OpCode::CB:
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
        case PrefixOpCode::SWAP_B:
            SWAP_r(RegisterOperand::B);
            break;
        case PrefixOpCode::SWAP_C:
            SWAP_r(RegisterOperand::C);
            break;
        case PrefixOpCode::SWAP_D:
            SWAP_r(RegisterOperand::D);
            break;
        case PrefixOpCode::SWAP_E:
            SWAP_r(RegisterOperand::E);
            break;
        case PrefixOpCode::SWAP_H:
            SWAP_r(RegisterOperand::H);
            break;
        case PrefixOpCode::SWAP_L:
            SWAP_r(RegisterOperand::L);
            break;
        case PrefixOpCode::SWAP_A:
            SWAP_r(RegisterOperand::A);
            break;
        case PrefixOpCode::SWAP_HL:
            SWAP_HL();
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