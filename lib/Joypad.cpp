#include <bigboy/Joypad.h>
#include <iostream>

void Joypad::handleInput(const KeyInput input) {
    updateKeystate(input);
    updateRegister();
}

void Joypad::updateKeystate(const KeyInput input) {
    switch (input) {
        case KeyInput::UP_PRESSED:
            m_keys.up = true;
            break;
        case KeyInput::UP_RELEASED:
            m_keys.up = false;
            break;
        case KeyInput::DOWN_PRESSED:
            m_keys.down = true;
            break;
        case KeyInput::DOWN_RELEASED:
            m_keys.down = false;
            break;
        case KeyInput::LEFT_PRESSED:
            m_keys.left = true;
            break;
        case KeyInput::LEFT_RELEASED:
            m_keys.left = false;
            break;
        case KeyInput::RIGHT_PRESSED:
            m_keys.right = true;
            break;
        case KeyInput::RIGHT_RELEASED:
            m_keys.right = false;
            break;
        case KeyInput::A_PRESSED:
            m_keys.a = true;
            break;
        case KeyInput::A_RELEASED:
            m_keys.a = false;
            break;
        case KeyInput::B_PRESSED:
            m_keys.b = true;
            break;
        case KeyInput::B_RELEASED:
            m_keys.b = false;
            break;
        case KeyInput::START_PRESSED:
            m_keys.start = true;
            break;
        case KeyInput::START_RELEASED:
            m_keys.start = false;
            break;
        case KeyInput::SELECT_PRESSED:
            m_keys.select = true;
            break;
        case KeyInput::SELECT_RELEASED:
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