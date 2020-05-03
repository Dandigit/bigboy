#include <bigboy/CPU/Registers.h>

uint8_t& Registers::get(RegisterOperand target) {
    switch (target) {
        case RegisterOperand::B: return b;
        case RegisterOperand::C: return c;
        case RegisterOperand::D: return d;
        case RegisterOperand::E: return e;
        case RegisterOperand::H: return h;
        case RegisterOperand::L: return l;
        case RegisterOperand::A: return a;
    }
}

uint16_t& Registers::get(RegisterPairOperand target) {
    switch (target) {
        case RegisterPairOperand::BC: return BC();
        case RegisterPairOperand::DE: return DE();
        case RegisterPairOperand::HL: return HL();
        case RegisterPairOperand::SP: return sp;
    }
}

uint16_t& Registers::get(RegisterPairStackOperand target) {
    switch (target) {
        case RegisterPairStackOperand::BC: return BC();
        case RegisterPairStackOperand::DE: return DE();
        case RegisterPairStackOperand::HL: return HL();
        case RegisterPairStackOperand::AF: return AF();
    }
}

uint16_t& Registers::AF() {
    return *static_cast<uint16_t*>(static_cast<void*>(&f));
}

uint16_t Registers::AF() const {
    return static_cast<uint16_t>(a) << 8u
           | static_cast<uint16_t>(f);
}

uint16_t& Registers::BC() {
    return *static_cast<uint16_t*>(static_cast<void*>(&c));
}

uint16_t Registers::BC() const {
    return static_cast<uint16_t>(b) << 8u
           | static_cast<uint16_t>(c);
}

uint16_t& Registers::DE() {
    return *static_cast<uint16_t*>(static_cast<void*>(&e));
}

uint16_t Registers::DE() const {
    return static_cast<uint16_t>(d) << 8u
           | static_cast<uint16_t>(e);
}

uint16_t& Registers::HL() {
    return *static_cast<uint16_t*>(static_cast<void*>(&l));
}

uint16_t Registers::HL() const {
    return static_cast<uint16_t>(h) << 8u
           | static_cast<uint16_t>(l);
}

uint16_t Registers::getBC() const {
    return static_cast<uint16_t>(b) << 8
           | static_cast<uint16_t>(c);
}

void Registers::setBC(uint16_t value) {
    b = static_cast<uint8_t>((value & 0xFF00) >> 8);
    c = static_cast<uint8_t>(value & 0xFF00);
}

uint16_t Registers::getDE() const {
    return static_cast<uint16_t>(d) << 8
           | static_cast<uint16_t>(e);
}

void Registers::setDE(uint16_t value) {
    d = static_cast<uint8_t>((value & 0xFF00) >> 8);
    e = static_cast<uint8_t>(value & 0xFF00);
}

uint16_t Registers::getHL() const {
    return static_cast<uint16_t>(h) << 8
           | static_cast<uint16_t>(l);
}

void Registers::setHL(uint16_t value) {
    h = static_cast<uint8_t>((value & 0xFF00) >> 8);
    l = static_cast<uint8_t>(value & 0xFF00);
}