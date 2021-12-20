#ifndef BIGBOY_EMULATOR_H
#define BIGBOY_EMULATOR_H

#include <bigboy/APU.h>
#include <bigboy/Cartridge.h>
#include <bigboy/CPU.h>
#include <bigboy/GPU.h>
#include <bigboy/Joypad.h>
#include <bigboy/Serial.h>
#include <bigboy/Timer.h>

class Emulator {
public:
    Emulator();
    void reset();

    const std::array<Colour, 160*144>& update();
    void handleInput(InputEvent event);

    bool loadRomFile(const std::string& path);
    bool loadRamFileIfSupported(const std::string& path);
    bool saveRamFileIfSupported(const std::string& path);

    std::string getGameTitle() const;

private:
    void step();

    CPU m_cpu{m_mmu};
    uint32_t m_clock = 0;

    MMU m_mmu{};
    GPU m_gpu{m_mmu};
    APU m_apu{};
    Joypad m_joypad{};
    Serial m_serial{};
    Timer m_timer{};

    std::unique_ptr<Cartridge> m_cartridge;
};

#endif //BIGBOY_EMULATOR_H
