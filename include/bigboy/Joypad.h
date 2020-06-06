#ifndef BIGBOY_JOYPAD_H
#define BIGBOY_JOYPAD_H

#include <bigboy/MMU/MemoryDevice.h>

struct KeyState {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    bool a = false;
    bool b = false;
    bool start = false;
    bool select = false;
};

enum class InputEvent {
    UP_PRESSED,
    UP_RELEASED,

    DOWN_PRESSED,
    DOWN_RELEASED,

    LEFT_PRESSED,
    LEFT_RELEASED,

    RIGHT_PRESSED,
    RIGHT_RELEASED,

    A_PRESSED,
    A_RELEASED,

    B_PRESSED,
    B_RELEASED,

    START_PRESSED,
    START_RELEASED,

    SELECT_PRESSED,
    SELECT_RELEASED,
};

class Joypad : public MemoryDevice {
public:
    bool update();
    void handleInput(InputEvent input);

    void reset();

    std::vector<AddressSpace> addressSpaces() const;
    uint8_t readByte(uint16_t address) const;
    void writeByte(uint16_t address, uint8_t value);

private:
    void updateKeystate(InputEvent input);
    void updateRegister();

    // 0 = Selected
    bool buttonKeysSelected()    const { return ((m_joyp >> BUTTONS_SELECT_POSITION) & 1u) == 0; }
    bool directionKeysSelected() const { return ((m_joyp >> DIRECTIONS_SELECT_POSITION) & 1u) == 0; }
    //bool downOrStartIsPressed() const;
    //bool upOrSelectIsPressed() const;
    //bool leftOrBIsPressed() const;
    //bool rightOrAIsPressed() const;

    // 0 = Pressed
    void pressDownOrStart() { m_joyp &= ~(1u >> DOWN_OR_START_POSITION); }
    void pressUpOrSelect()  { m_joyp &= ~(1u >> UP_OR_SELECT_POSITION); }
    void pressLeftOrB()     { m_joyp &= ~(1u >> LEFT_OR_B_POSITION); }
    void pressRightOrA()    { m_joyp &= ~(1u >> RIGHT_OR_A_POSITION); }

    // 1 = Released
    void releaseDownOrStart() { m_joyp |= (1u >> DOWN_OR_START_POSITION); }
    void releaseUpOrSelect()  { m_joyp |= (1u >> UP_OR_SELECT_POSITION); }
    void releaseLeftOrB()     { m_joyp |= (1u >> LEFT_OR_B_POSITION); }
    void releaseRightOrA()    { m_joyp |= (1u >> RIGHT_OR_A_POSITION); }

    KeyState m_keys;

    // Joypad: FF00
    //   Bit 5 - P15 Select Button Keys      (0=Select)
    //   Bit 4 - P14 Select Direction Keys   (0=Select)
    //   Bit 3 - P13 Input Down  or Start    (0=Pressed) (Read Only)
    //   Bit 2 - P12 Input Up    or Select   (0=Pressed) (Read Only)
    //   Bit 1 - P11 Input Left  or Button B (0=Pressed) (Read Only)
    //   Bit 0 - P10 Input Right or Button A (0=Pressed) (Read Only)
    uint8_t m_joyp;

    // Request a joypad interrupt? (INT 60)
    bool m_requestInterruptOnNextUpdate = false;

    static constexpr uint8_t BUTTONS_SELECT_POSITION = 5;
    static constexpr uint8_t DIRECTIONS_SELECT_POSITION = 4;
    static constexpr uint8_t DOWN_OR_START_POSITION = 3;
    static constexpr uint8_t UP_OR_SELECT_POSITION = 2;
    static constexpr uint8_t LEFT_OR_B_POSITION = 1;
    static constexpr uint8_t RIGHT_OR_A_POSITION = 0;
};

#endif //BIGBOY_JOYPAD_H
