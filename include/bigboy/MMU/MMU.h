#ifndef BIGBOY_MMU_H
#define BIGBOY_MMU_H

#include <vector>

#include <bigboy/MMU/InternalMemory.h>

class MMU {
    // MMU does own some general system memory that belongs nowhere else:
    InternalMemory m_internal;

    // We can initialise with the internal memory device already registered.
    std::vector<std::reference_wrapper<MemoryDevice>> m_devices{m_internal};

public:
    MMU() = default;
    MMU(std::initializer_list<std::reference_wrapper<MemoryDevice>> devices);
    ~MMU() = default;

    uint8_t readByte(uint16_t address) const;
    void writeByte(uint16_t address, uint8_t value);

    void registerDevice(MemoryDevice& device);
    void deregisterDevice(MemoryDevice& device);

    void reset();

private:
    MemoryDevice& getDevice(uint16_t address);
    const MemoryDevice& getDevice(uint16_t address) const;
};

#endif //BIGBOY_MMU_H
