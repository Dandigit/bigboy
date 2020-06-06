#include <bigboy/Joypad.h>
#include <iostream>

bool Joypad::update() {
    if (m_requestInterruptOnNextUpdate) {
        m_requestInterruptOnNextUpdate = false;
        return true;
    }

    return false;
}

void Joypad::handleInput(const InputEvent input) {
    updateKeystate(input);
    updateRegister();
}

void Joypad::updateKeystate(const InputEvent input) {
    switch (input) {
        case InputEvent::UP_PRESSED:
            if (!m_keys.up) m_requestInterruptOnNextUpdate = true;
            m_keys.up = true;
            break;
        case InputEvent::UP_RELEASED:
            m_keys.up = false;
            break;
        case InputEvent::DOWN_PRESSED:
            if (!m_keys.down) m_requestInterruptOnNextUpdate = true;
            m_keys.down = true;
            break;
        case InputEvent::DOWN_RELEASED:
            m_keys.down = false;
            break;
        case InputEvent::LEFT_PRESSED:
            if (!m_keys.left) m_requestInterruptOnNextUpdate = true;
            m_keys.left = true;
            break;
        case InputEvent::LEFT_RELEASED:
            m_keys.left = false;
            break;
        case InputEvent::RIGHT_PRESSED:
            if (!m_keys.right) m_requestInterruptOnNextUpdate = true;
            m_keys.right = true;
            break;
        case InputEvent::RIGHT_RELEASED:
            m_keys.right = false;
            break;
        case InputEvent::A_PRESSED:
            if (!m_keys.a) m_requestInterruptOnNextUpdate = true;
            m_keys.a = true;
            break;
        case InputEvent::A_RELEASED:
            m_keys.a = false;
            break;
        case InputEvent::B_PRESSED:
            if (!m_keys.b) m_requestInterruptOnNextUpdate = true;
            m_keys.b = true;
            break;
        case InputEvent::B_RELEASED:
            m_keys.b = false;
            break;
        case InputEvent::START_PRESSED:
            if (!m_keys.start) m_requestInterruptOnNextUpdate = true;
            m_keys.start = true;
            break;
        case InputEvent::START_RELEASED:
            m_keys.start = false;
            break;
        case InputEvent::SELECT_PRESSED:
            if (!m_keys.select) m_requestInterruptOnNextUpdate = true;
            m_keys.select = true;
            break;
        case InputEvent::SELECT_RELEASED:
            m_keys.select = false;
            break;
    }
}

void Joypad::updateRegister() {
    if (buttonKeysSelected()) {
        m_keys.start ? pressDownOrStart() : releaseDownOrStart();
        m_keys.select ? pressUpOrSelect() : releaseUpOrSelect();
        m_keys.b ? pressLeftOrB() : releaseLeftOrB();
        m_keys.a ? pressRightOrA() : releaseRightOrA();
    }

    if (directionKeysSelected()) {
        m_keys.down ? pressDownOrStart() : releaseDownOrStart();
        m_keys.up ? pressUpOrSelect() : releaseUpOrSelect();
        m_keys.left ? pressLeftOrB() : releaseLeftOrB();
        m_keys.right ? pressRightOrA() : releaseRightOrA();
    }
}

std::vector<AddressSpace> Joypad::addressSpaces() const {
    return { AddressSpace{0xFF00} };
}

uint8_t Joypad::readByte(const uint16_t address) const {
    if (address == 0xFF00) {
        return m_joyp;
    }

    std::cerr << "Memory device Joypad does not support reading the address " <<
            address << '\n';
    return 0xFF; // Bogus value
}

void Joypad::writeByte(const uint16_t address, const uint8_t value) {
    if (address == 0xFF00) {
        m_joyp = value;
    } else {
        std::cerr << "Memory device Joypad does not support writing to the address " <<
                  address << '\n';
        // Do nothing
    }
}