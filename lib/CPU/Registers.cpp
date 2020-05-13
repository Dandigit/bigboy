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

uint16_t& Registers::BC() {
    #ifdef BIGBOY_BIG_ENDIAN
    return *static_cast<uint16_t*>(static_cast<void*>(&b));
    #else
    return *static_cast<uint16_t*>(static_cast<void*>(&c));
    #endif
}

uint16_t Registers::BC() const {
    return static_cast<uint16_t>(b) << 8u
           | static_cast<uint16_t>(c);
}

uint16_t& Registers::DE() {
    #ifdef BIGBOY_BIG_ENDIAN
    return *static_cast<uint16_t*>(static_cast<void*>(&d));
    #else
    return *static_cast<uint16_t*>(static_cast<void*>(&e));
    #endif
}

uint16_t Registers::DE() const {
    return static_cast<uint16_t>(d) << 8u
           | static_cast<uint16_t>(e);
}

uint16_t& Registers::HL() {
    #ifdef BIGBOY_BIG_ENDIAN
    return *static_cast<uint16_t*>(static_cast<void*>(&h));
    #else
    return *static_cast<uint16_t*>(static_cast<void*>(&l));
    #endif
}

uint16_t Registers::HL() const {
    return static_cast<uint16_t>(h) << 8u
           | static_cast<uint16_t>(l);
}

uint16_t& Registers::AF() {
    #ifdef BIGBOY_BIG_ENDIAN
    return *static_cast<uint16_t*>(static_cast<void*>(&a));
    #else
    return *static_cast<uint16_t*>(static_cast<void*>(&f));
    #endif
}

uint16_t Registers::AF() const {
    return static_cast<uint16_t>(a) << 8u
           | static_cast<uint16_t>(f);
}