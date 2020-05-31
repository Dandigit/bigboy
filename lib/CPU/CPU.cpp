#include <bigboy/CPU/CPU.h>

#include <bitset>
#include <iostream>
#include <sstream>
#include <fstream>

void CPU::load(Cartridge cartridge) {
    reset();

    m_cartridge = cartridge;
    m_mmu.registerDevice(m_cartridge);
}

const std::array<Colour, 160*144>& CPU::stepFrame() {
    while (m_clock < 70224) {
        update();
    }

    m_clock -= 70224;
    return m_gpu.getCurrentFrame();
}

void CPU::update() {
    disassembleCurrent();
    const uint8_t cycles = step();

    m_clock += cycles;

    const bool timerRequest = m_timer.update(cycles);
    if (timerRequest) {
        requestInterrupt(Interrupt::TIMER);
    }

    const GPU::Request gpuRequest = m_gpu.update(cycles);
    if (gpuRequest.vblank) {
        requestInterrupt(Interrupt::VBLANK);
    }
    if (gpuRequest.stat) {
        requestInterrupt(Interrupt::LCD_STAT);
    }

    handleInterrupts();
}

void CPU::handleInterrupts() {
    if (!m_ime) return;

    for (int i = 0; i < INTERRUPT_COUNT; ++i) {
        auto interrupt = static_cast<Interrupt>(i);
        if (isInterruptServicable(interrupt)) {
            serviceInterrupt(interrupt);
            return;
        }
    }
}

void CPU::serviceInterrupt(Interrupt interrupt) {
    m_ime = false;
    unrequestInterrupt(interrupt);

    switch (interrupt) {
        case Interrupt::VBLANK:
            call(0x40);
            return;
        case Interrupt::LCD_STAT:
            call(0x48);
            return;
        case Interrupt::TIMER:
            call(0x50);
            return;
        case Interrupt::SERIAL:
            call(0x58);
            return;
        case Interrupt::JOYPAD:
            call(0x60);
            return;
    }
}

bool CPU::isInterruptServicable(Interrupt interrupt) const {
    return isInterruptEnabled(interrupt) && isInterruptRequested(interrupt);
}

bool CPU::isInterruptEnabled(Interrupt interrupt) const {
    uint8_t ie = m_mmu.readByte(0xFFFF);
    auto offset = static_cast<uint8_t>(interrupt);
    return ((ie >> offset) & 1u) != 0;
}

bool CPU::isInterruptRequested(Interrupt interrupt) const {
    uint8_t if_ = m_mmu.readByte(0xFF0F);
    auto offset = static_cast<uint8_t>(interrupt);
    return ((if_ >> offset) & 1u) != 0;
}

void CPU::enableInterrupt(Interrupt interrupt) {
    uint8_t ie = m_mmu.readByte(0xFFFF);
    auto offset = static_cast<uint8_t>(interrupt);

    ie |= (1u << offset);
    m_mmu.writeByte(0xFFFF, ie);
}

void CPU::disableInterrupt(Interrupt interrupt) {
    uint8_t ie = m_mmu.readByte(0xFFFF);
    auto offset = static_cast<uint8_t>(interrupt);

    ie &= ~(1u << offset);
    m_mmu.writeByte(0xFFFF, ie);
}

void CPU::requestInterrupt(Interrupt interrupt) {
    uint8_t if_ = m_mmu.readByte(0xFF0F);
    auto offset = static_cast<uint8_t>(interrupt);

    if_ |= (1u << offset);
    m_mmu.writeByte(0xFFFF, if_);
}

void CPU::unrequestInterrupt(Interrupt interrupt) {
    uint8_t if_ = m_mmu.readByte(0xFF0F);
    auto offset = static_cast<uint8_t>(interrupt);

    if_ &= ~(1u << offset);
    m_mmu.writeByte(0xFFFF, if_);
}

void CPU::reset() {
    m_mmu.reset();

    //m_cartridge.reset();
    m_mmu.registerDevice(m_cartridge);

    m_gpu.reset();
    m_mmu.registerDevice(m_gpu);

    //m_serial.reset();
    m_mmu.registerDevice(m_serial);

    m_timer.reset();
    m_mmu.registerDevice(m_timer);

    m_registers.reset();
    m_pc = 0x100;
    m_halted = false;
    m_stopped = false;
    m_ime = true;
}

std::string CPU::disassembleCurrent() {
    auto current = static_cast<OpCode>(m_mmu.readByte(m_pc));
    std::stringstream s;

    s << "-- DISASSEMBLY\n";
    s << "-- clock = " << m_clock << '\n';
    s << "-- " << opCodeToString(current)
            << '(' << (int)current << ") "
            << '[' << (int)(m_mmu.readByte(m_pc + 1)) << "]\n";
    s << "-- b = " << (int) m_registers.b <<
            ", c = " << (int) m_registers.c <<
            ", bc = " << m_registers.BC() << '\n';
    s << "-- d = " << (int) m_registers.d <<
            ", e = " << (int) m_registers.e <<
            ", de = " << m_registers.DE() << '\n';
    s << "-- h = " << (int) m_registers.h <<
            ", l = " << (int) m_registers.l <<
            ", hl = " << m_registers.HL() << '\n';
    s << "-- a = " << (int) m_registers.a <<
            ", f = " << (int) m_registers.f <<
            ", af = " << m_registers.AF() << '\n';
    s << "-- pc = " << m_pc << ", sp = " << m_registers.sp << '\n';
    s << "-- zero = " << getZeroFlag() <<
            ", subtract = " << getSubtractFlag() <<
            ", halfCarry = " << getHalfCarryFlag() <<
            ", carry = " << getCarryFlag() << '\n';
    s << "-- lcdc = " << (int)(m_mmu.readByte(0xFF40)) <<
            ", stat = " << (int)(m_mmu.readByte(0xFF41)) <<
            ", ly = " << (int)(m_mmu.readByte(0xFF44)) << '\n';

    std::ofstream outfile;
    outfile.open("/Users/dboulton/bigboy-dump.txt", std::ios_base::app);
    outfile << s.str();
    outfile.close();

    return s.str();
}

uint8_t CPU::nextByte() {
    return m_mmu.readByte(m_pc++);
}

uint16_t CPU::nextWord() {
    uint16_t word = m_mmu.readWord(m_pc);
    m_pc += 2;
    return word;
}

bool CPU::getCondition(ConditionOperand condition) const {
    switch (condition) {
        case ConditionOperand::NZ: return !getZeroFlag();
        case ConditionOperand::Z:  return getZeroFlag();
        case ConditionOperand::NC: return !getCarryFlag();
        case ConditionOperand::C:  return getCarryFlag();
    }
}

void CPU::load(uint8_t& target, uint8_t value) {
    target = value;
}

uint8_t CPU::LD_r_r(RegisterOperand target, RegisterOperand value) {
    load(m_registers.get(target), m_registers.get(value));
    return 4;
}

uint8_t CPU::LD_r_n(RegisterOperand target) {
    load(m_registers.get(target), nextByte());
    return 8;
}

uint8_t CPU::LD_r_HL(RegisterOperand target) {
    load(m_registers.get(target), m_mmu.readByte(m_registers.HL()));
    return 8;
}

uint8_t CPU::LD_HL_r(RegisterOperand value) {
    uint8_t dummy = m_mmu.readByte(m_registers.HL());
    load(dummy, m_registers.get(value));
    m_mmu.writeByte(m_registers.HL(), dummy);
    return 8;
}

uint8_t CPU::LD_HL_n() {
    uint8_t dummy = m_mmu.readByte(m_registers.HL());
    load(dummy, nextByte());
    m_mmu.writeByte(m_registers.HL(), dummy);
    return 12;
}

uint8_t CPU::LD_A_BC() {
    load(m_registers.a, m_mmu.readByte(m_registers.BC()));
    return 8;
}

uint8_t CPU::LD_A_DE() {
    load(m_registers.a, m_mmu.readByte(m_registers.DE()));
    return 8;
}

uint8_t CPU::LD_A_nn() {
    uint16_t nn = nextWord();

    load(m_registers.a, m_mmu.readByte(nn));
    return 16;
}

uint8_t CPU::LD_BC_A() {
    uint8_t dummy = m_mmu.readByte(m_registers.BC());
    load(dummy, m_registers.a);
    m_mmu.writeByte(m_registers.BC(), dummy);
    return 8;
}

uint8_t CPU::LD_DE_A() {
    m_mmu.writeByte(m_registers.DE(), m_registers.a);
    return 8;
}

uint8_t CPU::LD_nn_A() {
    uint16_t nn = nextWord();

    uint8_t dummy = m_mmu.readByte(nn);
    load(dummy, m_registers.a);
    m_mmu.writeByte(nn, dummy);
    return 16;
}

uint8_t CPU::LD_A_FF00n() {
    load(m_registers.a, m_mmu.readByte(0xFF00 + nextByte()));
    return 12;
}

uint8_t CPU::LD_FF00n_A() {
    uint16_t addr = 0xFF00 + nextByte();
    uint8_t dummy = m_mmu.readByte(addr);
    load(dummy, m_registers.a);
    m_mmu.writeByte(addr, dummy);
    return 12;
}

uint8_t CPU::LD_A_FF00C() {
    load(m_registers.a, m_mmu.readByte(0xFF00 + m_registers.c));
    return 8;
}

uint8_t CPU::LD_FF00C_A() {
    uint16_t addr = 0xFF00 + m_registers.c;
    uint8_t dummy = m_mmu.readByte(addr);
    load(dummy, m_registers.a);
    m_mmu.writeByte(addr, dummy);
    return 8;
}

uint8_t CPU::LDI_HL_A() {
    uint8_t dummy = m_mmu.readByte(m_registers.HL());
    load(dummy , m_registers.a);
    m_mmu.writeByte(m_registers.HL()++, dummy);
    return 8;
}

uint8_t CPU::LDI_A_HL() {
    load(m_registers.a, m_mmu.readByte(m_registers.HL()));
    ++m_registers.HL();
    return 8;
}

uint8_t CPU::LDD_HL_A() {
    uint8_t dummy = m_mmu.readByte(m_registers.HL());
    load(dummy , m_registers.a);
    m_mmu.writeByte(m_registers.HL()--, dummy);
    return 8;
}

uint8_t CPU::LDD_A_HL() {
    load(m_registers.a, m_mmu.readByte(m_registers.HL()));
    --m_registers.HL();
    return 8;
}

void CPU::load(uint16_t& target, uint16_t value) {
    target = value;
}

uint8_t CPU::LD_dd_nn(RegisterPairOperand target) {
    uint16_t nn = nextWord();

    load(m_registers.get(target), nn);
    return 12;
}

uint8_t CPU::LD_SP_HL() {
    load(m_registers.sp, m_registers.HL());
    return 8;
}

void CPU::push(uint16_t value) {
    uint8_t high = (value >> 8u);
    m_mmu.writeByte(--m_registers.sp, high);

    uint8_t low = (value & 0xFFu);
    m_mmu.writeByte(--m_registers.sp, low);
}

uint8_t CPU::PUSH_qq(RegisterPairStackOperand value) {
    push(m_registers.get(value));
    return 16;
}

void CPU::pop(uint16_t& target) {
    target = m_mmu.readWord(m_registers.sp);
    m_registers.sp += 2;
}

uint8_t CPU::POP_qq(RegisterPairStackOperand target) {
    pop(m_registers.get(target));

    if (target == RegisterPairStackOperand::AF) {
        m_registers.f &= 0xF0;
    }

    return 12;
}

// Add `value` to the register A, and set/reset the necessary flags
void CPU::add(uint8_t value) {
    uint8_t result = m_registers.a + value;

    result == 0 ? setZeroFlag() : clearZeroFlag();
    clearSubtractFlag();
    result < value ? setZeroFlag() : clearZeroFlag();
    (m_registers.a & 0xF) + (value & 0xF) > 0xF ? setHalfCarryFlag() : clearHalfCarryFlag();

    m_registers.a = result;
}

uint8_t CPU::ADDA_r(RegisterOperand target) {
    add(m_registers.get(target));
    return 4;
}

uint8_t CPU::ADDA_n() {
    add(nextByte());
    return 8;
}

uint8_t CPU::ADDA_HL() {
    add(m_mmu.readByte(m_registers.HL()));
    return 8;
}

// Add `value` plus the carry flag to the register A, and set/reset the necessary flags
void CPU::addWithCarry(uint8_t value) {
    add(value + (getCarryFlag() ? 1 : 0));
}

uint8_t CPU::ADCA_r(RegisterOperand target) {
    addWithCarry(m_registers.get(target));
    return 4;
}

uint8_t CPU::ADCA_n() {
    addWithCarry(nextByte());
    return 8;
}

uint8_t CPU::ADCA_HL() {
    addWithCarry(m_mmu.readByte(m_registers.HL()));
    return 8;
}

// Subtract `value` from the register A, set the correct flags,
// and store the result in register A
void CPU::subtract(uint8_t value) {
    uint8_t result = m_registers.a - value;

    result == 0 ? setCarryFlag() : clearCarryFlag();
    setSubtractFlag();
    result > value ? setCarryFlag() : clearCarryFlag();
    ((m_registers.a & 0x0F) < (value & 0x0F)) ? setHalfCarryFlag() : clearHalfCarryFlag();

    m_registers.a = result;
}

uint8_t CPU::SUB_r(RegisterOperand target) {
    subtract(m_registers.get(target));
    return 4;
}

uint8_t CPU::SUB_n() {
    subtract(nextByte());
    return 8;
}

uint8_t CPU::SUB_HL() {
    subtract(m_mmu.readByte(m_registers.HL()));
    return 8;
}

void CPU::subtractWithCarry(uint8_t value) {
    subtract(value + (getCarryFlag() ? 1 : 0));
}

uint8_t CPU::SBCA_r(RegisterOperand target) {
    subtractWithCarry(m_registers.get(target));
    return 4;
}

uint8_t CPU::SBCA_n() {
    subtractWithCarry(nextByte());
    return 8;
}

uint8_t CPU::SBCA_HL() {
    subtractWithCarry(m_mmu.readByte(m_registers.HL()));
    return 8;
}

void CPU::bitwiseAnd(uint8_t value) {
    m_registers.a &= value;

    m_registers.a == 0 ? setZeroFlag() : clearZeroFlag();
    clearSubtractFlag();
    clearCarryFlag();
    setHalfCarryFlag();
}

uint8_t CPU::AND_r(RegisterOperand target) {
    bitwiseAnd(m_registers.get(target));
    return 4;
}

uint8_t CPU::AND_n() {
    bitwiseAnd(nextByte());
    return 8;
}

uint8_t CPU::AND_HL() {
    bitwiseAnd(m_mmu.readByte(m_registers.HL()));
    return 8;
}

void CPU::bitwiseXor(uint8_t value) {
    m_registers.a ^= value;

    m_registers.a == 0 ? setZeroFlag() : clearZeroFlag();
    clearSubtractFlag();
    clearCarryFlag();
    clearHalfCarryFlag();
}

uint8_t CPU::XOR_r(RegisterOperand target) {
    bitwiseXor(m_registers.get(target));
    return 4;
}

uint8_t CPU::XOR_n() {
    bitwiseXor(nextByte());
    return 8;
}

uint8_t CPU::XOR_HL() {
    bitwiseXor(m_mmu.readByte(m_registers.HL()));
    return 8;
}

void CPU::bitwiseOr(uint8_t value) {
    m_registers.a |= value;

    (m_registers.a == 0) ? setZeroFlag() : clearZeroFlag();
    clearSubtractFlag();
    clearCarryFlag();
    clearHalfCarryFlag();
}

uint8_t CPU::OR_r(RegisterOperand target) {
    bitwiseOr(m_registers.get(target));
    return 4;
}

uint8_t CPU::OR_n() {
    bitwiseOr(nextByte());
    return 8;
}

uint8_t CPU::OR_HL() {
    bitwiseOr(m_mmu.readByte(m_registers.HL()));
    return 8;
}

// Compares `value` with (subtracts it from) the register A, setting the appropriate
// flags but not storing the result.
void CPU::compare(uint8_t value) {
    uint8_t result = m_registers.a - value;

    (result == 0) ? setZeroFlag() : clearZeroFlag();
    setSubtractFlag();
    (result > value) ? setCarryFlag() : clearCarryFlag();
    ((m_registers.a & 0x0F) < (value & 0x0F)) ? setHalfCarryFlag() : clearHalfCarryFlag();
}

uint8_t CPU::CP_r(RegisterOperand target) {
    compare(m_registers.get(target));
    return 4;
}

uint8_t CPU::CP_n() {
    compare(nextByte());
    return 8;
}

uint8_t CPU::CP_HL() {
    compare(m_mmu.readByte(m_registers.HL()));
    return 8;
}

void CPU::increment(uint8_t &target) {
    uint8_t result = target + 1;

    (result == 0) ? setZeroFlag() : clearZeroFlag();
    clearSubtractFlag();
    (((target >> 3u) & 1u) != 0 && (((result >> 3u) & 1u) == 0)) ? setHalfCarryFlag() : clearHalfCarryFlag();

    target = result;
}

uint8_t CPU::INC_r(RegisterOperand target) {
    increment(m_registers.get(target));
    return 4;
}

uint8_t CPU::INC_HL() {
    uint8_t dummy = m_mmu.readByte(m_registers.HL());
    increment(dummy);
    m_mmu.writeByte(m_registers.HL(), dummy);
    return 12;
}

void CPU::decrement(uint8_t& target) {
    uint8_t result = target - 1;

    (result == 0) ? setZeroFlag() : clearZeroFlag();
    setSubtractFlag();
    (((result ^ 0x01 ^ target) & 0x10) == 0x10) ? setHalfCarryFlag() : clearHalfCarryFlag();

    target = result;
}

uint8_t CPU::DEC_r(RegisterOperand target) {
    decrement(m_registers.get(target));
    return 4;
}

uint8_t CPU::DEC_HL_() {
    uint8_t dummy = m_mmu.readByte(m_registers.HL());
    decrement(dummy);
    m_mmu.writeByte(m_registers.HL(), dummy);
    return 12;
}

// Retroactively transform the previous addition or subtraction into a BCD operation
uint8_t CPU::DAA() {
    if (getSubtractFlag()) {
        // After a subtraction, only adjust if a carry and/or half carry occurred
        if (getCarryFlag()) {
            m_registers.a -= 0x60;
        }

        if (getHalfCarryFlag()) {
            m_registers.a -= 0x6;
        }
    } else {
        // After an addition, only adjust if a carry and/or half carry occurred
        // or if the result is out of bounds
        if (getCarryFlag() || m_registers.a > 0x99) {
            m_registers.a += 0x60;
            setCarryFlag();
        }

        if (getHalfCarryFlag() || (m_registers.a & 0x0f) > 0x09) {
            m_registers.a += 0x6;
        }
    }

    // Always update these flags
    (m_registers.a == 0) ? setZeroFlag() : clearZeroFlag();
    clearHalfCarryFlag();

    return 4;
}

// Invert the contents of register A
uint8_t CPU::CPL() {
    m_registers.a ^= 0xFF;

    setSubtractFlag();
    setHalfCarryFlag();

    return 4;
}

// Add `value` to `target`, storing the result in `target` and setting the appropriate flags
void CPU::add(uint16_t &target, uint16_t value) {
    uint16_t result = target + value;

    clearSubtractFlag();
    (result < target) ? setCarryFlag() : clearCarryFlag();
    (((result ^ target ^ value) & 0x1000) != 0) ? setHalfCarryFlag() : clearHalfCarryFlag();

    target = result;
}

uint8_t CPU::ADD_HL_rr(RegisterPairOperand value) {
    add(m_registers.HL(), m_registers.get(value));
    return 8;
}

void CPU::increment(uint16_t& target) {
    ++target;
}

uint8_t CPU::INC_rr(RegisterPairOperand target) {
    increment(m_registers.get(target));
    return 8;
}

void CPU::decrement(uint16_t& target) {
    --target;
}

uint8_t CPU::DEC_rr(RegisterPairOperand target) {
    decrement(m_registers.get(target));
    return 8;
}

void CPU::add(uint16_t &target, int8_t value) {
    uint16_t result = target + value;

    clearZeroFlag();
    clearSubtractFlag();
    ((result & 0xF) < (target & 0xF)) ? setHalfCarryFlag() : clearHalfCarryFlag();
    ((result & 0xFF) < (target & 0xFF)) ? setCarryFlag() : clearCarryFlag();

    target = result;
}

uint8_t CPU::ADD_SP_s() {
    auto s = static_cast<int8_t>(nextByte());
    add(m_registers.sp, s);
    return 16;
}

uint8_t CPU::LD_HL_SPs() {
    uint16_t value = m_registers.sp;
    auto s = static_cast<int8_t>(nextByte());
    add(value, s);

    load(m_registers.HL(), value);
    return 12;
}

// Rotate `target` left 1 bit position, copying the sign bit to the carry flag and bit 0
void CPU::rotateLeft(uint8_t &target) {
    uint8_t signBit = target >> 7u;

    target <<= 1u;
    target ^= signBit;

    (signBit != 0) ? setCarryFlag() : clearCarryFlag();
    clearHalfCarryFlag();
    clearSubtractFlag();
}

uint8_t CPU::RLCA() {
    rotateLeft(m_registers.a);
    return 4;
}

uint8_t CPU::RLC_r(RegisterOperand target) {
    rotateLeft(m_registers.get(target));
    return 8;
}

uint8_t CPU::RLC_HL() {
    uint8_t dummy = m_mmu.readByte(m_registers.HL());
    rotateLeft(dummy);
    m_mmu.writeByte(m_registers.HL(), dummy);
    return 16;
}

// Rotate `target` left 1 bit position through the carry flag,
// copying the previous contents of the carry flag to bit 0
void CPU::rotateLeftThroughCarry(uint8_t &target) {
    uint8_t prevCarryFlag = (getCarryFlag() ? 1 : 0);

    std::bitset<9> value{target};
    value[8] = prevCarryFlag;

    value <<= 1u;
    value[0] = prevCarryFlag;

    target = static_cast<uint8_t>(value.to_ulong());

    value[8] ? setCarryFlag() : clearCarryFlag();
    clearHalfCarryFlag();
    clearSubtractFlag();
}

uint8_t CPU::RLA() {
    rotateLeftThroughCarry(m_registers.a);
    return 4;
}

uint8_t CPU::RL_r(RegisterOperand target) {
    rotateLeftThroughCarry(m_registers.get(target));
    return 8;
}

uint8_t CPU::RL_HL() {
    uint8_t dummy = m_mmu.readByte(m_registers.HL());
    rotateLeftThroughCarry(dummy);
    m_mmu.writeByte(m_registers.HL(), dummy);
    return 16;
}

void CPU::rotateRight(uint8_t &target) {
    uint8_t zerothBit = target & 1u;

    target >>= 1u;
    target ^= (zerothBit << 7u);

    (zerothBit != 0) ? setCarryFlag() : clearCarryFlag();
    clearHalfCarryFlag();
    clearSubtractFlag();
}

uint8_t CPU::RRCA() {
    rotateRight(m_registers.a);
    return 4;
}

uint8_t CPU::RRC_r(RegisterOperand target) {
    rotateRight(m_registers.get(target));
    return 8;
}

uint8_t CPU::RRC_HL() {
    uint8_t dummy = m_mmu.readByte(m_registers.HL());
    rotateRight(dummy);
    m_mmu.writeByte(m_registers.HL(), dummy);
    return 16;
}

void CPU::rotateRightThroughCarry(uint8_t &target) {
    uint8_t prevCarryFlag = (getCarryFlag() ? 1 : 0);

    std::bitset<9> value{target};
    value <<= 1u;
    value[0] = prevCarryFlag;

    value >>= 1u;
    value[8] = prevCarryFlag;

    target = static_cast<uint8_t>((value >> 1).to_ulong());

    value[0] ? setCarryFlag() : clearCarryFlag();
    clearHalfCarryFlag();
    clearSubtractFlag();
}

uint8_t CPU::RRA() {
    rotateRightThroughCarry(m_registers.a);
    return 4;
}

uint8_t CPU::RR_r(RegisterOperand target) {
    rotateRightThroughCarry(m_registers.get(target));
    return 8;
}

uint8_t CPU::RR_HL() {
    uint8_t dummy = m_mmu.readByte(m_registers.HL());
    rotateRightThroughCarry(dummy);
    m_mmu.writeByte(m_registers.HL(), dummy);
    return 16;
}

// Shift `target` to the left by 1 bit position, after copying bit 7 into the carry flag
void CPU::shiftLeft(uint8_t& target) {
    ((target >> 7u) != 0) ? setCarryFlag() : clearCarryFlag();

    target <<= 1u;

    (target == 0) ? setZeroFlag() : clearZeroFlag();
    clearHalfCarryFlag();
    clearSubtractFlag();
}

uint8_t CPU::SLA_r(RegisterOperand target) {
    shiftLeft(m_registers.get(target));
    return 8;
}

uint8_t CPU::SLA_HL() {
    uint8_t dummy = m_mmu.readByte(m_registers.HL());
    shiftLeft(dummy);
    m_mmu.writeByte(m_registers.HL(), dummy);
    return 16;
}

// Swap the lower and upper nibbles of `target`, setting the appropriate
// flags.
void CPU::swap(uint8_t& target) {
    target = ((target & 0x0Fu) << 4u | (target & 0xF0u) >> 4u);

    (target == 0) ? setZeroFlag() : clearZeroFlag();
    clearSubtractFlag();
    clearHalfCarryFlag();
    clearCarryFlag();
}

uint8_t CPU::SWAP_r(RegisterOperand target) {
    swap(m_registers.get(target));
    return 8;
}

uint8_t CPU::SWAP_HL() {
    uint8_t dummy = m_mmu.readByte(m_registers.HL());
    swap(dummy);
    m_mmu.writeByte(m_registers.HL(), dummy);
    return 16;
}

// Shift the lower 7 bits (0-6) of `target` 1 bit position to the right,
// after copying bit 0 into the carry flag.
void CPU::shiftTailRight(uint8_t& target) {
    ((target & 1u) != 0) ? setCarryFlag() : clearCarryFlag();

    // Save the 7th bit
    uint8_t bit7 = (target >> 7u);

    // Shift the entire number right
    target >>= 1u;

    // Restore the 7th bit
    target |= (bit7 << 7u);

    (target == 0) ? setCarryFlag() : clearCarryFlag();
    clearHalfCarryFlag();
    clearSubtractFlag();
}

uint8_t CPU::SRA_r(RegisterOperand target) {
    shiftTailRight(m_registers.get(target));
    return 8;
}

uint8_t CPU::SRA_HL() {
    uint8_t dummy = m_mmu.readByte(m_registers.HL());
    shiftTailRight(dummy);
    m_mmu.writeByte(m_registers.HL(), dummy);
    return 16;
}

// Shift `target` to the right by 1 bit position, after copying bit 0 into the carry flag
void CPU::shiftRight(uint8_t& target) {
    ((target & 1u) != 0) ? setCarryFlag() : clearCarryFlag();

    target >>= 1u;

    (target == 0) ? setCarryFlag() : clearCarryFlag();
    clearHalfCarryFlag();
    clearSubtractFlag();
}

uint8_t CPU::SRL_r(RegisterOperand target) {
    shiftRight(m_registers.get(target));
    return 8;
}

uint8_t CPU::SRL_HL() {
    uint8_t dummy = m_mmu.readByte(m_registers.HL());
    shiftRight(dummy);
    m_mmu.writeByte(m_registers.HL(), dummy);
    return 16;
}

// Check if the bit at position `bit` in `byte` is zero. Set the zero flag accordingly.
void CPU::testBit(BitOperand bit, uint8_t byte) {
    uint8_t nthBit = ((byte >> static_cast<uint8_t>(bit)) & 1u);

    nthBit == 0 ? setZeroFlag() : clearZeroFlag();
    setHalfCarryFlag();
    clearSubtractFlag();
}

uint8_t CPU::BIT_b_r(BitOperand bit, RegisterOperand reg) {
    testBit(bit, m_registers.get(reg));
    return 8;
}

uint8_t CPU::BIT_b_HL(BitOperand bit) {
    testBit(bit, m_mmu.readByte(m_registers.HL()));
    return 12;
}

// Set the bit at position `bit` in `target`. No flags are affected.
void CPU::setBit(BitOperand bit, uint8_t &target) {
    target |= (1u << static_cast<uint8_t>(bit));
}

uint8_t CPU::SET_b_r(BitOperand bit, RegisterOperand reg) {
    setBit(bit, m_registers.get(reg));
    return 8;
}

uint8_t CPU::SET_b_HL(BitOperand bit) {
    uint8_t dummy = m_mmu.readByte(m_registers.HL());
    setBit(bit, dummy);
    m_mmu.writeByte(m_registers.HL(), dummy);
    return 16;
}

// Reset the bit at position `bit` in `target`. No flags are affected.
void CPU::resetBit(BitOperand bit, uint8_t &target) {
    target &= ~(1u << static_cast<uint8_t>(bit));
}

uint8_t CPU::RES_b_r(BitOperand bit, RegisterOperand reg) {
    resetBit(bit, m_registers.get(reg));
    return 8;
}

uint8_t CPU::RES_b_HL(BitOperand bit) {
    uint8_t dummy = m_mmu.readByte(m_registers.HL());
    resetBit(bit, dummy);
    m_mmu.writeByte(m_registers.HL(), dummy);
    return 16;
}

// Invert the carry flag
uint8_t CPU::CCF() {
    getCarryFlag() ? clearCarryFlag() : setCarryFlag();

    clearSubtractFlag();
    clearHalfCarryFlag();

    return 4;
}

// Set the carry flag
uint8_t CPU::SCF() {
    setCarryFlag();

    clearSubtractFlag();
    clearHalfCarryFlag();

    return 4;
}

uint8_t CPU::NOP() {
    // Nothing!
    return 4;
}

uint8_t CPU::HALT() {
    m_halted = true;
    return 4;
}

uint8_t CPU::STOP() {
    m_stopped = true;
    return 0;
}

uint8_t CPU::DI() {
    m_ime = false;
    return 4;
}

uint8_t CPU::EI() {
    m_ime = true;
    return 4;
}

void CPU::absoluteJump(uint16_t address) {
    load(m_pc, address);
}

uint8_t CPU::JP_nn() {
    uint16_t nn = nextWord();

    absoluteJump(nn);
    return 16;
}

uint8_t CPU::JP_HL() {
    absoluteJump(m_registers.HL());
    return 4;
}

uint8_t CPU::JP_f_nn(ConditionOperand condition) {
    uint16_t nn = nextWord();

    if (getCondition(condition)) {
        absoluteJump(nn);
        return 16;
    }

    return 12;
}

void CPU::relativeJump(int8_t offset) {
    m_pc += offset;
}

uint8_t CPU::JR_PCdd() {
    auto dd = static_cast<int8_t>(nextByte());
    relativeJump(dd);
    return 12;
}

uint8_t CPU::JR_f_PCdd(ConditionOperand condition) {
    auto dd = static_cast<int8_t>(nextByte());
    if (getCondition(condition)) {
        relativeJump(dd);
        return 12;
    }

    return 8;
}

void CPU::call(uint16_t address) {
    push(m_pc);
    absoluteJump(address);
}

uint8_t CPU::CALL_nn() {
    uint16_t nn = nextWord();

    call(nn);
    return 24;
}

uint8_t CPU::CALL_f_nn(ConditionOperand condition) {
    uint16_t nn = nextWord();

    if (getCondition(condition)) {
        call(nn);
        return 24;
    }

    return 12;
}

void CPU::ret() {
    pop(m_pc);
}

uint8_t CPU::RET() {
    ret();
    return 16;
}

uint8_t CPU::RET_f(ConditionOperand condition) {
    if (getCondition(condition)) {
        ret();
        return 20;
    }

    return 8;
}

uint8_t CPU::RETI() {
    ret();
    m_ime = true;
    return 16;
}

uint8_t CPU::RST(ResetOperand address) {
    call(static_cast<uint16_t>(address));
    return 16;
}

uint8_t CPU::step() {
    if (m_stopped) return 0;
    if (m_halted) {
        // We need to keep the clock going.
        return NOP();
    }

    auto current = static_cast<OpCode>(nextByte());

    switch (current) {
        case OpCode::LD_B_B:
            return LD_r_r(RegisterOperand::B, RegisterOperand::B);
        case OpCode::LD_B_C:
            return LD_r_r(RegisterOperand::B, RegisterOperand::C);
        case OpCode::LD_B_D:
            return LD_r_r(RegisterOperand::B, RegisterOperand::D);
        case OpCode::LD_B_E:
            return LD_r_r(RegisterOperand::B, RegisterOperand::E);
        case OpCode::LD_B_H:
            return LD_r_r(RegisterOperand::B, RegisterOperand::H);
        case OpCode::LD_B_L:
            return LD_r_r(RegisterOperand::B, RegisterOperand::L);
        case OpCode::LD_B_A:
            return LD_r_r(RegisterOperand::B, RegisterOperand::A);
        case OpCode::LD_C_B:
            return LD_r_r(RegisterOperand::C, RegisterOperand::B);
        case OpCode::LD_C_C:
            return LD_r_r(RegisterOperand::C, RegisterOperand::C);
        case OpCode::LD_C_D:
            return LD_r_r(RegisterOperand::C, RegisterOperand::D);
        case OpCode::LD_C_E:
            return LD_r_r(RegisterOperand::C, RegisterOperand::E);
        case OpCode::LD_C_H:
            return LD_r_r(RegisterOperand::C, RegisterOperand::H);
        case OpCode::LD_C_L:
            return LD_r_r(RegisterOperand::C, RegisterOperand::L);
        case OpCode::LD_C_A:
            return LD_r_r(RegisterOperand::C, RegisterOperand::A);
        case OpCode::LD_D_B:
            return LD_r_r(RegisterOperand::D, RegisterOperand::B);
        case OpCode::LD_D_C:
            return LD_r_r(RegisterOperand::D, RegisterOperand::C);
        case OpCode::LD_D_D:
            return LD_r_r(RegisterOperand::D, RegisterOperand::D);
        case OpCode::LD_D_E:
            return LD_r_r(RegisterOperand::D, RegisterOperand::E);
        case OpCode::LD_D_H:
            return LD_r_r(RegisterOperand::D, RegisterOperand::H);
        case OpCode::LD_D_L:
            return LD_r_r(RegisterOperand::D, RegisterOperand::L);
        case OpCode::LD_D_A:
            return LD_r_r(RegisterOperand::D, RegisterOperand::A);
        case OpCode::LD_E_B:
            return LD_r_r(RegisterOperand::E, RegisterOperand::B);
        case OpCode::LD_E_C:
            return LD_r_r(RegisterOperand::E, RegisterOperand::C);
        case OpCode::LD_E_D:
            return LD_r_r(RegisterOperand::E, RegisterOperand::D);
        case OpCode::LD_E_E:
            return LD_r_r(RegisterOperand::E, RegisterOperand::E);
        case OpCode::LD_E_H:
            return LD_r_r(RegisterOperand::E, RegisterOperand::H);
        case OpCode::LD_E_L:
            return LD_r_r(RegisterOperand::E, RegisterOperand::L);
        case OpCode::LD_E_A:
            return LD_r_r(RegisterOperand::E, RegisterOperand::A);
        case OpCode::LD_H_B:
            return LD_r_r(RegisterOperand::H, RegisterOperand::B);
        case OpCode::LD_H_C:
            return LD_r_r(RegisterOperand::H, RegisterOperand::C);
        case OpCode::LD_H_D:
            return LD_r_r(RegisterOperand::H, RegisterOperand::D);
        case OpCode::LD_H_E:
            return LD_r_r(RegisterOperand::H, RegisterOperand::E);
        case OpCode::LD_H_H:
            return LD_r_r(RegisterOperand::H, RegisterOperand::H);
        case OpCode::LD_H_L:
            return LD_r_r(RegisterOperand::H, RegisterOperand::L);
        case OpCode::LD_H_A:
            return LD_r_r(RegisterOperand::H, RegisterOperand::A);
        case OpCode::LD_L_B:
            return LD_r_r(RegisterOperand::L, RegisterOperand::B);
        case OpCode::LD_L_C:
            return LD_r_r(RegisterOperand::L, RegisterOperand::C);
        case OpCode::LD_L_D:
            return LD_r_r(RegisterOperand::L, RegisterOperand::D);
        case OpCode::LD_L_E:
            return LD_r_r(RegisterOperand::L, RegisterOperand::E);
        case OpCode::LD_L_H:
            return LD_r_r(RegisterOperand::L, RegisterOperand::H);
        case OpCode::LD_L_L:
            return LD_r_r(RegisterOperand::L, RegisterOperand::L);
        case OpCode::LD_L_A:
            return LD_r_r(RegisterOperand::L, RegisterOperand::A);
        case OpCode::LD_A_B:
            return LD_r_r(RegisterOperand::A, RegisterOperand::B);
        case OpCode::LD_A_C:
            return LD_r_r(RegisterOperand::A, RegisterOperand::C);
        case OpCode::LD_A_D:
            return LD_r_r(RegisterOperand::A, RegisterOperand::D);
        case OpCode::LD_A_E:
            return LD_r_r(RegisterOperand::A, RegisterOperand::E);
        case OpCode::LD_A_H:
            return LD_r_r(RegisterOperand::A, RegisterOperand::H);
        case OpCode::LD_A_L:
            return LD_r_r(RegisterOperand::A, RegisterOperand::L);
        case OpCode::LD_A_A:
            return LD_r_r(RegisterOperand::A, RegisterOperand::A);
        case OpCode::LD_B_n:
            return LD_r_n(RegisterOperand::B);
        case OpCode::LD_C_n:
            return LD_r_n(RegisterOperand::C);
        case OpCode::LD_D_n:
            return LD_r_n(RegisterOperand::D);
        case OpCode::LD_E_n:
            return LD_r_n(RegisterOperand::E);
        case OpCode::LD_H_n:
            return LD_r_n(RegisterOperand::H);
        case OpCode::LD_L_n:
            return LD_r_n(RegisterOperand::L);
        case OpCode::LD_A_n:
            return LD_r_n(RegisterOperand::A);
        case OpCode::LD_B_HL:
            return LD_r_HL(RegisterOperand::B);
        case OpCode::LD_C_HL:
            return LD_r_HL(RegisterOperand::C);
        case OpCode::LD_D_HL:
            return LD_r_HL(RegisterOperand::D);
        case OpCode::LD_E_HL:
            return LD_r_HL(RegisterOperand::E);
        case OpCode::LD_H_HL:
            return LD_r_HL(RegisterOperand::H);
        case OpCode::LD_L_HL:
            return LD_r_HL(RegisterOperand::L);
        case OpCode::LD_A_HL:
            return LD_r_HL(RegisterOperand::A);
        case OpCode::LD_HL_B:
            return LD_HL_r(RegisterOperand::B);
        case OpCode::LD_HL_C:
            return LD_HL_r(RegisterOperand::C);
        case OpCode::LD_HL_D:
            return LD_HL_r(RegisterOperand::D);
        case OpCode::LD_HL_E:
            return LD_HL_r(RegisterOperand::E);
        case OpCode::LD_HL_H:
            return LD_HL_r(RegisterOperand::H);
        case OpCode::LD_HL_L:
            return LD_HL_r(RegisterOperand::L);
        case OpCode::LD_HL_A:
            return LD_HL_r(RegisterOperand::A);
        case OpCode::LD_HL_n:
            return LD_HL_n();
        case OpCode::LD_A_BC:
            return LD_A_BC();
        case OpCode::LD_A_DE:
            return LD_A_DE();
        case OpCode::LD_A_nn:
            return LD_A_nn();
        case OpCode::LD_BC_A:
            return LD_BC_A();
        case OpCode::LD_DE_A:
            return LD_DE_A();
        case OpCode::LD_nn_A:
            return LD_nn_A();
        case OpCode::LD_A_FF00n:
            return LD_A_FF00n();
        case OpCode::LD_FF00n_A:
            return LD_FF00n_A();
        case OpCode::LD_A_FF00C:
            return LD_A_FF00C();
        case OpCode::LD_FF00C_A:
            return LD_FF00C_A();
        case OpCode::LDI_HL_A:
            return LDI_HL_A();
        case OpCode::LDI_A_HL:
            return LDI_A_HL();
        case OpCode::LDD_HL_A:
            return LDD_HL_A();
        case OpCode::LDD_A_HL:
            return LDD_A_HL();
        case OpCode::LD_BC_nn:
            return LD_dd_nn(RegisterPairOperand::BC);
        case OpCode::LD_DE_nn:
            return LD_dd_nn(RegisterPairOperand::DE);
        case OpCode::LD_HL_nn:
            return LD_dd_nn(RegisterPairOperand::HL);
        case OpCode::LD_SP_nn:
            return LD_dd_nn(RegisterPairOperand::SP);
        case OpCode::LD_SP_HL:
            return LD_SP_HL();
        case OpCode::PUSH_BC:
            return PUSH_qq(RegisterPairStackOperand::BC);
        case OpCode::PUSH_DE:
            return PUSH_qq(RegisterPairStackOperand::DE);
        case OpCode::PUSH_HL:
            return PUSH_qq(RegisterPairStackOperand::HL);
        case OpCode::PUSH_AF:
            return PUSH_qq(RegisterPairStackOperand::AF);
        case OpCode::POP_BC:
            return POP_qq(RegisterPairStackOperand::BC);
        case OpCode::POP_DE:
            return POP_qq(RegisterPairStackOperand::DE);
        case OpCode::POP_HL:
            return POP_qq(RegisterPairStackOperand::HL);
        case OpCode::POP_AF:
            return POP_qq(RegisterPairStackOperand::AF);
        case OpCode::ADDA_B:
            return ADDA_r(RegisterOperand::B);
        case OpCode::ADDA_C:
            return ADDA_r(RegisterOperand::C);
        case OpCode::ADDA_D:
            return ADDA_r(RegisterOperand::D);
        case OpCode::ADDA_E:
            return ADDA_r(RegisterOperand::E);
        case OpCode::ADDA_H:
            return ADDA_r(RegisterOperand::H);
        case OpCode::ADDA_L:
            return ADDA_r(RegisterOperand::L);
        case OpCode::ADDA_A:
            return ADDA_r(RegisterOperand::A);
        case OpCode::ADDA_n:
            return ADDA_n();
        case OpCode::ADDA_HL:
            return ADDA_HL();
        case OpCode::ADCA_B:
            return ADCA_r(RegisterOperand::B);
        case OpCode::ADCA_C:
            return ADCA_r(RegisterOperand::C);
        case OpCode::ADCA_D:
            return ADCA_r(RegisterOperand::D);
        case OpCode::ADCA_E:
            return ADCA_r(RegisterOperand::E);
        case OpCode::ADCA_H:
            return ADCA_r(RegisterOperand::H);
        case OpCode::ADCA_L:
            return ADCA_r(RegisterOperand::L);
        case OpCode::ADCA_A:
            return ADCA_r(RegisterOperand::A);
        case OpCode::ADCA_n:
            return ADCA_n();
        case OpCode::ADCA_HL:
            return ADCA_HL();
        case OpCode::SUB_B:
            return SUB_r(RegisterOperand::B);
        case OpCode::SUB_C:
            return SUB_r(RegisterOperand::C);
        case OpCode::SUB_D:
            return SUB_r(RegisterOperand::D);
        case OpCode::SUB_E:
            return SUB_r(RegisterOperand::E);
        case OpCode::SUB_H:
            return SUB_r(RegisterOperand::H);
        case OpCode::SUB_L:
            return SUB_r(RegisterOperand::L);
        case OpCode::SUB_A:
            return SUB_r(RegisterOperand::A);
        case OpCode::SUB_n:
            return SUB_n();
        case OpCode::SUB_HL:
            return SUB_HL();
        case OpCode::SBCA_B:
            return SBCA_r(RegisterOperand::B);
        case OpCode::SBCA_C:
            return SBCA_r(RegisterOperand::C);
        case OpCode::SBCA_D:
            return SBCA_r(RegisterOperand::D);
        case OpCode::SBCA_E:
            return SBCA_r(RegisterOperand::E);
        case OpCode::SBCA_H:
            return SBCA_r(RegisterOperand::H);
        case OpCode::SBCA_L:
            return SBCA_r(RegisterOperand::L);
        case OpCode::SBCA_A:
            return SBCA_r(RegisterOperand::A);
        case OpCode::SBCA_n:
            return SBCA_n();
        case OpCode::SBCA_HL:
            return SBCA_HL();
        case OpCode::AND_B:
            return AND_r(RegisterOperand::B);
        case OpCode::AND_C:
            return AND_r(RegisterOperand::C);
        case OpCode::AND_D:
            return AND_r(RegisterOperand::D);
        case OpCode::AND_E:
            return AND_r(RegisterOperand::E);
        case OpCode::AND_H:
            return AND_r(RegisterOperand::H);
        case OpCode::AND_L:
            return AND_r(RegisterOperand::L);
        case OpCode::AND_A:
            return AND_r(RegisterOperand::A);
        case OpCode::AND_n:
            return AND_n();
        case OpCode::AND_HL:
            return AND_HL();
        case OpCode::XOR_B:
            return XOR_r(RegisterOperand::B);
        case OpCode::XOR_C:
            return XOR_r(RegisterOperand::C);
        case OpCode::XOR_D:
            return XOR_r(RegisterOperand::D);
        case OpCode::XOR_E:
            return XOR_r(RegisterOperand::E);
        case OpCode::XOR_H:
            return XOR_r(RegisterOperand::H);
        case OpCode::XOR_L:
            return XOR_r(RegisterOperand::L);
        case OpCode::XOR_A:
            return XOR_r(RegisterOperand::A);
        case OpCode::XOR_n:
            return XOR_n();
        case OpCode::XOR_HL:
            return XOR_HL();
        case OpCode::OR_B:
            return OR_r(RegisterOperand::B);
        case OpCode::OR_C:
            return OR_r(RegisterOperand::C);
        case OpCode::OR_D:
            return OR_r(RegisterOperand::D);
        case OpCode::OR_E:
            return OR_r(RegisterOperand::E);
        case OpCode::OR_H:
            return OR_r(RegisterOperand::H);
        case OpCode::OR_L:
            return OR_r(RegisterOperand::L);
        case OpCode::OR_A:
            return OR_r(RegisterOperand::A);
        case OpCode::OR_n:
            return OR_n();
        case OpCode::OR_HL:
            return OR_HL();
        case OpCode::CP_B:
            return CP_r(RegisterOperand::B);
        case OpCode::CP_C:
            return CP_r(RegisterOperand::C);
        case OpCode::CP_D:
            return CP_r(RegisterOperand::D);
        case OpCode::CP_E:
            return CP_r(RegisterOperand::E);
        case OpCode::CP_H:
            return CP_r(RegisterOperand::H);
        case OpCode::CP_L:
            return CP_r(RegisterOperand::L);
        case OpCode::CP_A:
            return CP_r(RegisterOperand::A);
        case OpCode::CP_n:
            return CP_n();
        case OpCode::CP_HL:
            return CP_HL();
        case OpCode::INC_B:
            return INC_r(RegisterOperand::B);
        case OpCode::INC_C:
            return INC_r(RegisterOperand::C);
        case OpCode::INC_D:
            return INC_r(RegisterOperand::D);
        case OpCode::INC_E:
            return INC_r(RegisterOperand::E);
        case OpCode::INC_H:
            return INC_r(RegisterOperand::H);
        case OpCode::INC_L:
            return INC_r(RegisterOperand::L);
        case OpCode::INC_A:
            return INC_r(RegisterOperand::A);
        case OpCode::INC_HL_:
            return INC_HL();
        case OpCode::DEC_B:
            return DEC_r(RegisterOperand::B);
        case OpCode::DEC_C:
            return DEC_r(RegisterOperand::C);
        case OpCode::DEC_D:
            return DEC_r(RegisterOperand::D);
        case OpCode::DEC_E:
            return DEC_r(RegisterOperand::E);
        case OpCode::DEC_H:
            return DEC_r(RegisterOperand::H);
        case OpCode::DEC_L:
            return DEC_r(RegisterOperand::L);
        case OpCode::DEC_A:
            return DEC_r(RegisterOperand::A);
        case OpCode::DEC_HL_:
            return DEC_HL_();
        case OpCode::DAA:
            return DAA();
        case OpCode::CPL:
            return CPL();
        case OpCode::ADD_HL_BC:
            return ADD_HL_rr(RegisterPairOperand::BC);
        case OpCode::ADD_HL_DE:
            return ADD_HL_rr(RegisterPairOperand::DE);
        case OpCode::ADD_HL_HL:
            return ADD_HL_rr(RegisterPairOperand::HL);
        case OpCode::ADD_HL_SP:
            return ADD_HL_rr(RegisterPairOperand::SP);
        case OpCode::INC_BC:
            return INC_rr(RegisterPairOperand::BC);
        case OpCode::INC_DE:
            return INC_rr(RegisterPairOperand::DE);
        case OpCode::INC_HL:
            return INC_rr(RegisterPairOperand::HL);
        case OpCode::INC_SP:
            return INC_rr(RegisterPairOperand::SP);
        case OpCode::DEC_BC:
            return DEC_rr(RegisterPairOperand::BC);
        case OpCode::DEC_DE:
            return DEC_rr(RegisterPairOperand::DE);
        case OpCode::DEC_HL:
            return DEC_rr(RegisterPairOperand::HL);
        case OpCode::DEC_SP:
            return DEC_rr(RegisterPairOperand::SP);
        case OpCode::ADD_SP_s:
            return ADD_SP_s();
        case OpCode::LD_HL_SPs:
            return LD_HL_SPs();
        case OpCode::RLCA:
            return RLCA();
        case OpCode::RLA:
            return RLA();
        case OpCode::RRCA:
            return RRCA();
        case OpCode::RRA:
            return RRA();
        case OpCode::CCF:
            return CCF();
        case OpCode::SCF:
            return SCF();
        case OpCode::NOP:
            return NOP();
        case OpCode::HALT:
            return HALT();
        case OpCode::STOP:
            return STOP();
        case OpCode::DI:
            return DI();
        case OpCode::EI:
            return EI();
        case OpCode::JP_nn:
            return JP_nn();
        case OpCode::JP_HL:
            return JP_HL();
        case OpCode::JP_NZ_nn:
            return JP_f_nn(ConditionOperand::NZ);
        case OpCode::JP_Z_nn:
            return JP_f_nn(ConditionOperand::Z);
        case OpCode::JP_NC_nn:
            return JP_f_nn(ConditionOperand::NC);
        case OpCode::JP_C_nn:
            return JP_f_nn(ConditionOperand::C);
        case OpCode::JR_PCdd:
            return JR_PCdd();
        case OpCode::JR_NZ_PCdd:
            return JR_f_PCdd(ConditionOperand::NZ);
        case OpCode::JR_Z_PCdd:
            return JR_f_PCdd(ConditionOperand::Z);
        case OpCode::JR_NC_PCdd:
            return JR_f_PCdd(ConditionOperand::NC);
        case OpCode::JR_C_PCdd:
            return JR_f_PCdd(ConditionOperand::C);
        case OpCode::CALL_nn:
            return CALL_nn();
        case OpCode::CALL_NZ_nn:
            return CALL_f_nn(ConditionOperand::NZ);
        case OpCode::CALL_Z_nn:
            return CALL_f_nn(ConditionOperand::Z);
        case OpCode::CALL_NC_nn:
            return CALL_f_nn(ConditionOperand::NC);
        case OpCode::CALL_C_nn:
            return CALL_f_nn(ConditionOperand::C);
        case OpCode::RET:
            return RET();
        case OpCode::RET_NZ:
            return RET_f(ConditionOperand::NZ);
        case OpCode::RET_Z:
            return RET_f(ConditionOperand::Z);
        case OpCode::RET_NC:
            return RET_f(ConditionOperand::NC);
        case OpCode::RET_C:
            return RET_f(ConditionOperand::C);
        case OpCode::RETI:
            return RETI();
        case OpCode::RST_00:
            return RST(ResetOperand::x00);
        case OpCode::RST_08:
            return RST(ResetOperand::x08);
        case OpCode::RST_10:
            return RST(ResetOperand::x10);
        case OpCode::RST_18:
            return RST(ResetOperand::x18);
        case OpCode::RST_20:
            return RST(ResetOperand::x20);
        case OpCode::RST_28:
            return RST(ResetOperand::x28);
        case OpCode::RST_30:
            return RST(ResetOperand::x30);
        case OpCode::RST_38:
            return RST(ResetOperand::x38);
        case OpCode::CB:
            return stepPrefix();
        default:
            std::cerr << "Unknown instructon: " << std::bitset<8>{static_cast<uint8_t>(current)} << ".\nTerminating bigboy...\n";
            exit(1);
    }
}

uint8_t CPU::stepPrefix() {
    auto current = static_cast<PrefixOpCode>(nextByte());
    switch (current) {
        case PrefixOpCode::RLC_B:
            return RLC_r(RegisterOperand::B);
        case PrefixOpCode::RLC_C:
            return RLC_r(RegisterOperand::C);
        case PrefixOpCode::RLC_D:
            return RLC_r(RegisterOperand::D);
        case PrefixOpCode::RLC_E:
            return RLC_r(RegisterOperand::E);
        case PrefixOpCode::RLC_H:
            return RLC_r(RegisterOperand::H);
        case PrefixOpCode::RLC_L:
            return RLC_r(RegisterOperand::L);
        case PrefixOpCode::RLC_A:
            return RLC_r(RegisterOperand::A);
        case PrefixOpCode::RLC_HL:
            return RLC_HL();
        case PrefixOpCode::RL_B:
            return RL_r(RegisterOperand::B);
        case PrefixOpCode::RL_C:
            return RL_r(RegisterOperand::C);
        case PrefixOpCode::RL_D:
            return RL_r(RegisterOperand::D);
        case PrefixOpCode::RL_E:
            return RL_r(RegisterOperand::E);
        case PrefixOpCode::RL_H:
            return RL_r(RegisterOperand::H);
        case PrefixOpCode::RL_L:
            return RL_r(RegisterOperand::L);
        case PrefixOpCode::RL_A:
            return RL_r(RegisterOperand::A);
        case PrefixOpCode::RL_HL:
            return RL_HL();
        case PrefixOpCode::RRC_B:
            return RRC_r(RegisterOperand::B);
        case PrefixOpCode::RRC_C:
            return RRC_r(RegisterOperand::C);
        case PrefixOpCode::RRC_D:
            return RRC_r(RegisterOperand::D);
        case PrefixOpCode::RRC_E:
            return RRC_r(RegisterOperand::E);
        case PrefixOpCode::RRC_H:
            return RRC_r(RegisterOperand::H);
        case PrefixOpCode::RRC_L:
            return RRC_r(RegisterOperand::L);
        case PrefixOpCode::RRC_A:
            return RRC_r(RegisterOperand::A);
        case PrefixOpCode::RRC_HL:
            return RRC_HL();
        case PrefixOpCode::RR_B:
            return RR_r(RegisterOperand::B);
        case PrefixOpCode::RR_C:
            return RR_r(RegisterOperand::C);
        case PrefixOpCode::RR_D:
            return RR_r(RegisterOperand::D);
        case PrefixOpCode::RR_E:
            return RR_r(RegisterOperand::E);
        case PrefixOpCode::RR_H:
            return RR_r(RegisterOperand::H);
        case PrefixOpCode::RR_L:
            return RR_r(RegisterOperand::L);
        case PrefixOpCode::RR_A:
            return RR_r(RegisterOperand::A);
        case PrefixOpCode::RR_HL:
            return RR_HL();
        case PrefixOpCode::SLA_B:
            return SLA_r(RegisterOperand::B);
        case PrefixOpCode::SLA_C:
            return SLA_r(RegisterOperand::C);
        case PrefixOpCode::SLA_D:
            return SLA_r(RegisterOperand::D);
        case PrefixOpCode::SLA_E:
            return SLA_r(RegisterOperand::E);
        case PrefixOpCode::SLA_H:
            return SLA_r(RegisterOperand::H);
        case PrefixOpCode::SLA_L:
            return SLA_r(RegisterOperand::L);
        case PrefixOpCode::SLA_A:
            return SLA_r(RegisterOperand::A);
        case PrefixOpCode::SLA_HL:
            return SLA_HL();
        case PrefixOpCode::SWAP_B:
            return SWAP_r(RegisterOperand::B);
        case PrefixOpCode::SWAP_C:
            return SWAP_r(RegisterOperand::C);
        case PrefixOpCode::SWAP_D:
            return SWAP_r(RegisterOperand::D);
        case PrefixOpCode::SWAP_E:
            return SWAP_r(RegisterOperand::E);
        case PrefixOpCode::SWAP_H:
            return SWAP_r(RegisterOperand::H);
        case PrefixOpCode::SWAP_L:
            return SWAP_r(RegisterOperand::L);
        case PrefixOpCode::SWAP_A:
            return SWAP_r(RegisterOperand::A);
        case PrefixOpCode::SWAP_HL:
            return SWAP_HL();
        case PrefixOpCode::SRA_B:
            return SRA_r(RegisterOperand::B);
        case PrefixOpCode::SRA_C:
            return SRA_r(RegisterOperand::C);
        case PrefixOpCode::SRA_D:
            return SRA_r(RegisterOperand::D);
        case PrefixOpCode::SRA_E:
            return SRA_r(RegisterOperand::E);
        case PrefixOpCode::SRA_H:
            return SRA_r(RegisterOperand::H);
        case PrefixOpCode::SRA_L:
            return SRA_r(RegisterOperand::L);
        case PrefixOpCode::SRA_A:
            return SRA_r(RegisterOperand::A);
        case PrefixOpCode::SRA_HL:
            return SRA_HL();
        case PrefixOpCode::SRL_B:
            return SRL_r(RegisterOperand::B);
        case PrefixOpCode::SRL_C:
            return SRL_r(RegisterOperand::C);
        case PrefixOpCode::SRL_D:
            return SRL_r(RegisterOperand::D);
        case PrefixOpCode::SRL_E:
            return SRL_r(RegisterOperand::E);
        case PrefixOpCode::SRL_H:
            return SRL_r(RegisterOperand::H);
        case PrefixOpCode::SRL_L:
            return SRL_r(RegisterOperand::L);
        case PrefixOpCode::SRL_A:
            return SRL_r(RegisterOperand::A);
        case PrefixOpCode::SRL_HL:
            return SRL_HL();
        case PrefixOpCode::BIT_0_B:
            return BIT_b_r(BitOperand::BIT0, RegisterOperand::B);
        case PrefixOpCode::BIT_1_B:
            return BIT_b_r(BitOperand::BIT1, RegisterOperand::B);
        case PrefixOpCode::BIT_2_B:
            return BIT_b_r(BitOperand::BIT2, RegisterOperand::B);
        case PrefixOpCode::BIT_3_B:
            return BIT_b_r(BitOperand::BIT3, RegisterOperand::B);
        case PrefixOpCode::BIT_4_B:
            return BIT_b_r(BitOperand::BIT4, RegisterOperand::B);
        case PrefixOpCode::BIT_5_B:
            return BIT_b_r(BitOperand::BIT5, RegisterOperand::B);
        case PrefixOpCode::BIT_6_B:
            return BIT_b_r(BitOperand::BIT6, RegisterOperand::B);
        case PrefixOpCode::BIT_7_B:
            return BIT_b_r(BitOperand::BIT7, RegisterOperand::B);
        case PrefixOpCode::BIT_0_C:
            return BIT_b_r(BitOperand::BIT0, RegisterOperand::C);
        case PrefixOpCode::BIT_1_C:
            return BIT_b_r(BitOperand::BIT1, RegisterOperand::C);
        case PrefixOpCode::BIT_2_C:
            return BIT_b_r(BitOperand::BIT2, RegisterOperand::C);
        case PrefixOpCode::BIT_3_C:
            return BIT_b_r(BitOperand::BIT3, RegisterOperand::C);
        case PrefixOpCode::BIT_4_C:
            return BIT_b_r(BitOperand::BIT4, RegisterOperand::C);
        case PrefixOpCode::BIT_5_C:
            return BIT_b_r(BitOperand::BIT5, RegisterOperand::C);
        case PrefixOpCode::BIT_6_C:
            return BIT_b_r(BitOperand::BIT6, RegisterOperand::C);
        case PrefixOpCode::BIT_7_C:
            return BIT_b_r(BitOperand::BIT7, RegisterOperand::C);
        case PrefixOpCode::BIT_0_D:
            return BIT_b_r(BitOperand::BIT0, RegisterOperand::D);
        case PrefixOpCode::BIT_1_D:
            return BIT_b_r(BitOperand::BIT1, RegisterOperand::D);
        case PrefixOpCode::BIT_2_D:
            return BIT_b_r(BitOperand::BIT2, RegisterOperand::D);
        case PrefixOpCode::BIT_3_D:
            return BIT_b_r(BitOperand::BIT3, RegisterOperand::D);
        case PrefixOpCode::BIT_4_D:
            return BIT_b_r(BitOperand::BIT4, RegisterOperand::D);
        case PrefixOpCode::BIT_5_D:
            return BIT_b_r(BitOperand::BIT5, RegisterOperand::D);
        case PrefixOpCode::BIT_6_D:
            return BIT_b_r(BitOperand::BIT6, RegisterOperand::D);
        case PrefixOpCode::BIT_7_D:
            return BIT_b_r(BitOperand::BIT7, RegisterOperand::D);
        case PrefixOpCode::BIT_0_E:
            return BIT_b_r(BitOperand::BIT0, RegisterOperand::E);
        case PrefixOpCode::BIT_1_E:
            return BIT_b_r(BitOperand::BIT1, RegisterOperand::E);
        case PrefixOpCode::BIT_2_E:
            return BIT_b_r(BitOperand::BIT2, RegisterOperand::E);
        case PrefixOpCode::BIT_3_E:
            return BIT_b_r(BitOperand::BIT3, RegisterOperand::E);
        case PrefixOpCode::BIT_4_E:
            return BIT_b_r(BitOperand::BIT4, RegisterOperand::E);
        case PrefixOpCode::BIT_5_E:
            return BIT_b_r(BitOperand::BIT5, RegisterOperand::E);
        case PrefixOpCode::BIT_6_E:
            return BIT_b_r(BitOperand::BIT6, RegisterOperand::E);
        case PrefixOpCode::BIT_7_E:
            return BIT_b_r(BitOperand::BIT7, RegisterOperand::E);
        case PrefixOpCode::BIT_0_H:
            return BIT_b_r(BitOperand::BIT0, RegisterOperand::H);
        case PrefixOpCode::BIT_1_H:
            return BIT_b_r(BitOperand::BIT1, RegisterOperand::H);
        case PrefixOpCode::BIT_2_H:
            return BIT_b_r(BitOperand::BIT2, RegisterOperand::H);
        case PrefixOpCode::BIT_3_H:
            return BIT_b_r(BitOperand::BIT3, RegisterOperand::H);
        case PrefixOpCode::BIT_4_H:
            return BIT_b_r(BitOperand::BIT4, RegisterOperand::H);
        case PrefixOpCode::BIT_5_H:
            return BIT_b_r(BitOperand::BIT5, RegisterOperand::H);
        case PrefixOpCode::BIT_6_H:
            return BIT_b_r(BitOperand::BIT6, RegisterOperand::H);
        case PrefixOpCode::BIT_7_H:
            return BIT_b_r(BitOperand::BIT7, RegisterOperand::H);
        case PrefixOpCode::BIT_0_L:
            return BIT_b_r(BitOperand::BIT0, RegisterOperand::L);
        case PrefixOpCode::BIT_1_L:
            return BIT_b_r(BitOperand::BIT1, RegisterOperand::L);
        case PrefixOpCode::BIT_2_L:
            return BIT_b_r(BitOperand::BIT2, RegisterOperand::L);
        case PrefixOpCode::BIT_3_L:
            return BIT_b_r(BitOperand::BIT3, RegisterOperand::L);
        case PrefixOpCode::BIT_4_L:
            return BIT_b_r(BitOperand::BIT4, RegisterOperand::L);
        case PrefixOpCode::BIT_5_L:
            return BIT_b_r(BitOperand::BIT5, RegisterOperand::L);
        case PrefixOpCode::BIT_6_L:
            return BIT_b_r(BitOperand::BIT6, RegisterOperand::L);
        case PrefixOpCode::BIT_7_L:
            return BIT_b_r(BitOperand::BIT7, RegisterOperand::L);
        case PrefixOpCode::BIT_0_A:
            return BIT_b_r(BitOperand::BIT0, RegisterOperand::A);
        case PrefixOpCode::BIT_1_A:
            return BIT_b_r(BitOperand::BIT1, RegisterOperand::A);
        case PrefixOpCode::BIT_2_A:
            return BIT_b_r(BitOperand::BIT2, RegisterOperand::A);
        case PrefixOpCode::BIT_3_A:
            return BIT_b_r(BitOperand::BIT3, RegisterOperand::A);
        case PrefixOpCode::BIT_4_A:
            return BIT_b_r(BitOperand::BIT4, RegisterOperand::A);
        case PrefixOpCode::BIT_5_A:
            return BIT_b_r(BitOperand::BIT5, RegisterOperand::A);
        case PrefixOpCode::BIT_6_A:
            return BIT_b_r(BitOperand::BIT6, RegisterOperand::A);
        case PrefixOpCode::BIT_7_A:
            return BIT_b_r(BitOperand::BIT7, RegisterOperand::A);
        case PrefixOpCode::BIT_0_HL:
            return BIT_b_HL(BitOperand::BIT0);
        case PrefixOpCode::BIT_1_HL:
            return BIT_b_HL(BitOperand::BIT1);
        case PrefixOpCode::BIT_2_HL:
            return BIT_b_HL(BitOperand::BIT2);
        case PrefixOpCode::BIT_3_HL:
            return BIT_b_HL(BitOperand::BIT3);
        case PrefixOpCode::BIT_4_HL:
            return BIT_b_HL(BitOperand::BIT4);
        case PrefixOpCode::BIT_5_HL:
            return BIT_b_HL(BitOperand::BIT5);
        case PrefixOpCode::BIT_6_HL:
            return BIT_b_HL(BitOperand::BIT6);
        case PrefixOpCode::BIT_7_HL:
            return BIT_b_HL(BitOperand::BIT7);
        case PrefixOpCode::SET_0_B:
            return SET_b_r(BitOperand::BIT0, RegisterOperand::B);
        case PrefixOpCode::SET_1_B:
            return SET_b_r(BitOperand::BIT1, RegisterOperand::B);
        case PrefixOpCode::SET_2_B:
            return SET_b_r(BitOperand::BIT2, RegisterOperand::B);
        case PrefixOpCode::SET_3_B:
            return SET_b_r(BitOperand::BIT3, RegisterOperand::B);
        case PrefixOpCode::SET_4_B:
            return SET_b_r(BitOperand::BIT4, RegisterOperand::B);
        case PrefixOpCode::SET_5_B:
            return SET_b_r(BitOperand::BIT5, RegisterOperand::B);
        case PrefixOpCode::SET_6_B:
            return SET_b_r(BitOperand::BIT6, RegisterOperand::B);
        case PrefixOpCode::SET_7_B:
            return SET_b_r(BitOperand::BIT7, RegisterOperand::B);
        case PrefixOpCode::SET_0_C:
            return SET_b_r(BitOperand::BIT0, RegisterOperand::C);
        case PrefixOpCode::SET_1_C:
            return SET_b_r(BitOperand::BIT1, RegisterOperand::C);
        case PrefixOpCode::SET_2_C:
            return SET_b_r(BitOperand::BIT2, RegisterOperand::C);
        case PrefixOpCode::SET_3_C:
            return SET_b_r(BitOperand::BIT3, RegisterOperand::C);
        case PrefixOpCode::SET_4_C:
            return SET_b_r(BitOperand::BIT4, RegisterOperand::C);
        case PrefixOpCode::SET_5_C:
            return SET_b_r(BitOperand::BIT5, RegisterOperand::C);
        case PrefixOpCode::SET_6_C:
            return SET_b_r(BitOperand::BIT6, RegisterOperand::C);
        case PrefixOpCode::SET_7_C:
            return SET_b_r(BitOperand::BIT7, RegisterOperand::C);
        case PrefixOpCode::SET_0_D:
            return SET_b_r(BitOperand::BIT0, RegisterOperand::D);
        case PrefixOpCode::SET_1_D:
            return SET_b_r(BitOperand::BIT1, RegisterOperand::D);
        case PrefixOpCode::SET_2_D:
            return SET_b_r(BitOperand::BIT2, RegisterOperand::D);
        case PrefixOpCode::SET_3_D:
            return SET_b_r(BitOperand::BIT3, RegisterOperand::D);
        case PrefixOpCode::SET_4_D:
            return SET_b_r(BitOperand::BIT4, RegisterOperand::D);
        case PrefixOpCode::SET_5_D:
            return SET_b_r(BitOperand::BIT5, RegisterOperand::D);
        case PrefixOpCode::SET_6_D:
            return SET_b_r(BitOperand::BIT6, RegisterOperand::D);
        case PrefixOpCode::SET_7_D:
            return SET_b_r(BitOperand::BIT7, RegisterOperand::D);
        case PrefixOpCode::SET_0_E:
            return SET_b_r(BitOperand::BIT0, RegisterOperand::E);
        case PrefixOpCode::SET_1_E:
            return SET_b_r(BitOperand::BIT1, RegisterOperand::E);
        case PrefixOpCode::SET_2_E:
            return SET_b_r(BitOperand::BIT2, RegisterOperand::E);
        case PrefixOpCode::SET_3_E:
            return SET_b_r(BitOperand::BIT3, RegisterOperand::E);
        case PrefixOpCode::SET_4_E:
            return SET_b_r(BitOperand::BIT4, RegisterOperand::E);
        case PrefixOpCode::SET_5_E:
            return SET_b_r(BitOperand::BIT5, RegisterOperand::E);
        case PrefixOpCode::SET_6_E:
            return SET_b_r(BitOperand::BIT6, RegisterOperand::E);
        case PrefixOpCode::SET_7_E:
            return SET_b_r(BitOperand::BIT7, RegisterOperand::E);
        case PrefixOpCode::SET_0_H:
            return SET_b_r(BitOperand::BIT0, RegisterOperand::H);
        case PrefixOpCode::SET_1_H:
            return SET_b_r(BitOperand::BIT1, RegisterOperand::H);
        case PrefixOpCode::SET_2_H:
            return SET_b_r(BitOperand::BIT2, RegisterOperand::H);
        case PrefixOpCode::SET_3_H:
            return SET_b_r(BitOperand::BIT3, RegisterOperand::H);
        case PrefixOpCode::SET_4_H:
            return SET_b_r(BitOperand::BIT4, RegisterOperand::H);
        case PrefixOpCode::SET_5_H:
            return SET_b_r(BitOperand::BIT5, RegisterOperand::H);
        case PrefixOpCode::SET_6_H:
            return SET_b_r(BitOperand::BIT6, RegisterOperand::H);
        case PrefixOpCode::SET_7_H:
            return SET_b_r(BitOperand::BIT7, RegisterOperand::H);
        case PrefixOpCode::SET_0_L:
            return SET_b_r(BitOperand::BIT0, RegisterOperand::L);
        case PrefixOpCode::SET_1_L:
            return SET_b_r(BitOperand::BIT1, RegisterOperand::L);
        case PrefixOpCode::SET_2_L:
            return SET_b_r(BitOperand::BIT2, RegisterOperand::L);
        case PrefixOpCode::SET_3_L:
            return SET_b_r(BitOperand::BIT3, RegisterOperand::L);
        case PrefixOpCode::SET_4_L:
            return SET_b_r(BitOperand::BIT4, RegisterOperand::L);
        case PrefixOpCode::SET_5_L:
            return SET_b_r(BitOperand::BIT5, RegisterOperand::L);
        case PrefixOpCode::SET_6_L:
            return SET_b_r(BitOperand::BIT6, RegisterOperand::L);
        case PrefixOpCode::SET_7_L:
            return SET_b_r(BitOperand::BIT7, RegisterOperand::L);
        case PrefixOpCode::SET_0_A:
            return SET_b_r(BitOperand::BIT0, RegisterOperand::A);
        case PrefixOpCode::SET_1_A:
            return SET_b_r(BitOperand::BIT1, RegisterOperand::A);
        case PrefixOpCode::SET_2_A:
            return SET_b_r(BitOperand::BIT2, RegisterOperand::A);
        case PrefixOpCode::SET_3_A:
            return SET_b_r(BitOperand::BIT3, RegisterOperand::A);
        case PrefixOpCode::SET_4_A:
            return SET_b_r(BitOperand::BIT4, RegisterOperand::A);
        case PrefixOpCode::SET_5_A:
            return SET_b_r(BitOperand::BIT5, RegisterOperand::A);
        case PrefixOpCode::SET_6_A:
            return SET_b_r(BitOperand::BIT6, RegisterOperand::A);
        case PrefixOpCode::SET_7_A:
            return SET_b_r(BitOperand::BIT7, RegisterOperand::A);
        case PrefixOpCode::SET_0_HL:
            return SET_b_HL(BitOperand::BIT0);
        case PrefixOpCode::SET_1_HL:
            return SET_b_HL(BitOperand::BIT1);
        case PrefixOpCode::SET_2_HL:
            return SET_b_HL(BitOperand::BIT2);
        case PrefixOpCode::SET_3_HL:
            return SET_b_HL(BitOperand::BIT3);
        case PrefixOpCode::SET_4_HL:
            return SET_b_HL(BitOperand::BIT4);
        case PrefixOpCode::SET_5_HL:
            return SET_b_HL(BitOperand::BIT5);
        case PrefixOpCode::SET_6_HL:
            return SET_b_HL(BitOperand::BIT6);
        case PrefixOpCode::SET_7_HL:
            return SET_b_HL(BitOperand::BIT7);
        case PrefixOpCode::RES_0_B:
            return RES_b_r(BitOperand::BIT0, RegisterOperand::B);
        case PrefixOpCode::RES_1_B:
            return RES_b_r(BitOperand::BIT1, RegisterOperand::B);
        case PrefixOpCode::RES_2_B:
            return RES_b_r(BitOperand::BIT2, RegisterOperand::B);
        case PrefixOpCode::RES_3_B:
            return RES_b_r(BitOperand::BIT3, RegisterOperand::B);
        case PrefixOpCode::RES_4_B:
            return RES_b_r(BitOperand::BIT4, RegisterOperand::B);
        case PrefixOpCode::RES_5_B:
            return RES_b_r(BitOperand::BIT5, RegisterOperand::B);
        case PrefixOpCode::RES_6_B:
            return RES_b_r(BitOperand::BIT6, RegisterOperand::B);
        case PrefixOpCode::RES_7_B:
            return RES_b_r(BitOperand::BIT7, RegisterOperand::B);
        case PrefixOpCode::RES_0_C:
            return RES_b_r(BitOperand::BIT0, RegisterOperand::C);
        case PrefixOpCode::RES_1_C:
            return RES_b_r(BitOperand::BIT1, RegisterOperand::C);
        case PrefixOpCode::RES_2_C:
            return RES_b_r(BitOperand::BIT2, RegisterOperand::C);
        case PrefixOpCode::RES_3_C:
            return RES_b_r(BitOperand::BIT3, RegisterOperand::C);
        case PrefixOpCode::RES_4_C:
            return RES_b_r(BitOperand::BIT4, RegisterOperand::C);
        case PrefixOpCode::RES_5_C:
            return RES_b_r(BitOperand::BIT5, RegisterOperand::C);
        case PrefixOpCode::RES_6_C:
            return RES_b_r(BitOperand::BIT6, RegisterOperand::C);
        case PrefixOpCode::RES_7_C:
            return RES_b_r(BitOperand::BIT7, RegisterOperand::C);
        case PrefixOpCode::RES_0_D:
            return RES_b_r(BitOperand::BIT0, RegisterOperand::D);
        case PrefixOpCode::RES_1_D:
            return RES_b_r(BitOperand::BIT1, RegisterOperand::D);
        case PrefixOpCode::RES_2_D:
            return RES_b_r(BitOperand::BIT2, RegisterOperand::D);
        case PrefixOpCode::RES_3_D:
            return RES_b_r(BitOperand::BIT3, RegisterOperand::D);
        case PrefixOpCode::RES_4_D:
            return RES_b_r(BitOperand::BIT4, RegisterOperand::D);
        case PrefixOpCode::RES_5_D:
            return RES_b_r(BitOperand::BIT5, RegisterOperand::D);
        case PrefixOpCode::RES_6_D:
            return RES_b_r(BitOperand::BIT6, RegisterOperand::D);
        case PrefixOpCode::RES_7_D:
            return RES_b_r(BitOperand::BIT7, RegisterOperand::D);
        case PrefixOpCode::RES_0_E:
            return RES_b_r(BitOperand::BIT0, RegisterOperand::E);
        case PrefixOpCode::RES_1_E:
            return RES_b_r(BitOperand::BIT1, RegisterOperand::E);
        case PrefixOpCode::RES_2_E:
            return RES_b_r(BitOperand::BIT2, RegisterOperand::E);
        case PrefixOpCode::RES_3_E:
            return RES_b_r(BitOperand::BIT3, RegisterOperand::E);
        case PrefixOpCode::RES_4_E:
            return RES_b_r(BitOperand::BIT4, RegisterOperand::E);
        case PrefixOpCode::RES_5_E:
            return RES_b_r(BitOperand::BIT5, RegisterOperand::E);
        case PrefixOpCode::RES_6_E:
            return RES_b_r(BitOperand::BIT6, RegisterOperand::E);
        case PrefixOpCode::RES_7_E:
            return RES_b_r(BitOperand::BIT7, RegisterOperand::E);
        case PrefixOpCode::RES_0_H:
            return RES_b_r(BitOperand::BIT0, RegisterOperand::H);
        case PrefixOpCode::RES_1_H:
            return RES_b_r(BitOperand::BIT1, RegisterOperand::H);
        case PrefixOpCode::RES_2_H:
            return RES_b_r(BitOperand::BIT2, RegisterOperand::H);
        case PrefixOpCode::RES_3_H:
            return RES_b_r(BitOperand::BIT3, RegisterOperand::H);
        case PrefixOpCode::RES_4_H:
            return RES_b_r(BitOperand::BIT4, RegisterOperand::H);
        case PrefixOpCode::RES_5_H:
            return RES_b_r(BitOperand::BIT5, RegisterOperand::H);
        case PrefixOpCode::RES_6_H:
            return RES_b_r(BitOperand::BIT6, RegisterOperand::H);
        case PrefixOpCode::RES_7_H:
            return RES_b_r(BitOperand::BIT7, RegisterOperand::H);
        case PrefixOpCode::RES_0_L:
            return RES_b_r(BitOperand::BIT0, RegisterOperand::L);
        case PrefixOpCode::RES_1_L:
            return RES_b_r(BitOperand::BIT1, RegisterOperand::L);
        case PrefixOpCode::RES_2_L:
            return RES_b_r(BitOperand::BIT2, RegisterOperand::L);
        case PrefixOpCode::RES_3_L:
            return RES_b_r(BitOperand::BIT3, RegisterOperand::L);
        case PrefixOpCode::RES_4_L:
            return RES_b_r(BitOperand::BIT4, RegisterOperand::L);
        case PrefixOpCode::RES_5_L:
            return RES_b_r(BitOperand::BIT5, RegisterOperand::L);
        case PrefixOpCode::RES_6_L:
            return RES_b_r(BitOperand::BIT6, RegisterOperand::L);
        case PrefixOpCode::RES_7_L:
            return RES_b_r(BitOperand::BIT7, RegisterOperand::L);
        case PrefixOpCode::RES_0_A:
            return RES_b_r(BitOperand::BIT0, RegisterOperand::A);
        case PrefixOpCode::RES_1_A:
            return RES_b_r(BitOperand::BIT1, RegisterOperand::A);
        case PrefixOpCode::RES_2_A:
            return RES_b_r(BitOperand::BIT2, RegisterOperand::A);
        case PrefixOpCode::RES_3_A:
            return RES_b_r(BitOperand::BIT3, RegisterOperand::A);
        case PrefixOpCode::RES_4_A:
            return RES_b_r(BitOperand::BIT4, RegisterOperand::A);
        case PrefixOpCode::RES_5_A:
            return RES_b_r(BitOperand::BIT5, RegisterOperand::A);
        case PrefixOpCode::RES_6_A:
            return RES_b_r(BitOperand::BIT6, RegisterOperand::A);
        case PrefixOpCode::RES_7_A:
            return RES_b_r(BitOperand::BIT7, RegisterOperand::A);
        case PrefixOpCode::RES_0_HL:
            return RES_b_HL(BitOperand::BIT0);
        case PrefixOpCode::RES_1_HL:
            return RES_b_HL(BitOperand::BIT1);
        case PrefixOpCode::RES_2_HL:
            return RES_b_HL(BitOperand::BIT2);
        case PrefixOpCode::RES_3_HL:
            return RES_b_HL(BitOperand::BIT3);
        case PrefixOpCode::RES_4_HL:
            return RES_b_HL(BitOperand::BIT4);
        case PrefixOpCode::RES_5_HL:
            return RES_b_HL(BitOperand::BIT5);
        case PrefixOpCode::RES_6_HL:
            return RES_b_HL(BitOperand::BIT6);
        case PrefixOpCode::RES_7_HL:
            return RES_b_HL(BitOperand::BIT7);
        default:
            std::cerr << "Unknown prefix instructon: " << std::bitset<8>{static_cast<uint8_t>(current)} << ".\nTerminating bigboy...\n";
            exit(1);
    }
}