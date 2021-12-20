#include <bigboy/Emulator.h>

Emulator::Emulator() {
    reset();
}

void Emulator::reset() {
    m_clock = 0;

    m_cpu.reset();
    m_cartridge.reset();
    m_gpu.reset();
    m_apu.reset();
    m_joypad.reset();
    m_timer.reset();
    //m_serial.reset();

    m_mmu.reset();
    m_mmu.registerDevice(m_gpu);
    m_mmu.registerDevice(m_apu);
    m_mmu.registerDevice(m_joypad);
    m_mmu.registerDevice(m_timer);
    m_mmu.registerDevice(m_serial);
}

Frame Emulator::update() {
    while (m_clock < 70224) {
        step();
    }

    m_clock -= 70224;
    return {m_gpu.getCurrentFrame(), m_apu.collectSamples()};
}

void Emulator::step() {
    const uint8_t cycles = m_cpu.step();
    m_clock += cycles;

    m_apu.update(cycles);

    const bool joypadRequest = m_joypad.update();
    if (joypadRequest) {
        m_cpu.requestInterrupt(Interrupt::JOYPAD);
    }

    const bool timerRequest = m_timer.update(cycles);
    if (timerRequest) {
        m_cpu.requestInterrupt(Interrupt::TIMER);
    }

    const GPU::Request gpuRequest = m_gpu.update(cycles);
    if (gpuRequest.vblank) {
        m_cpu.requestInterrupt(Interrupt::VBLANK);
    }
    if (gpuRequest.stat) {
        m_cpu.requestInterrupt(Interrupt::LCD_STAT);
    }

    m_cpu.handleInterrupts();
}

void Emulator::handleInput(InputEvent event) {
    m_joypad.handleInput(event);
}

bool Emulator::loadRomFile(const std::string& path) {
    m_cartridge = ::loadRomFile(path);
    m_mmu.registerDevice(*m_cartridge);
    return m_cartridge != nullptr;
}

bool Emulator::loadRamFileIfSupported(const std::string& path) {
    return m_cartridge->loadRamFileIfSupported(path);
}

bool Emulator::saveRamFileIfSupported(const std::string& path) {
    return m_cartridge->saveRamFileIfSupported(path);
}

std::string Emulator::getGameTitle() const {
    return m_cartridge->getGameTitle();
}
