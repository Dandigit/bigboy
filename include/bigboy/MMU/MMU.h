#ifndef BIGBOY_MMU_H
#define BIGBOY_MMU_H

#include <vector>

#include <bigboy/MMU/InternalMemory.h>

class MMU {
    // MMU does own some general system memory that belongs nowhere else:
    InternalMemory m_internal;

    // We have to use pointers rather than reference wrappers for default construction
    std::vector<MemoryDevice*> m_devices{0xFFFF + 1, nullptr};

public:
    MMU();
    MMU(std::initializer_list<std::reference_wrapper<MemoryDevice>> devices);
    ~MMU() = default;

    uint8_t readByte(uint16_t address) const;
    void writeByte(uint16_t address, uint8_t value);

    uint16_t readWord(uint16_t address) const;
    void writeWord(uint16_t address, uint16_t value);

    void registerDevice(MemoryDevice& device);

    void reset();

private:
    MemoryDevice& getDevice(uint16_t address);
    const MemoryDevice& getDevice(uint16_t address) const;

    void reserveAddressSpace(MemoryDevice& device, AddressSpace addressSpace);
};

#endif //BIGBOY_MMU_H
