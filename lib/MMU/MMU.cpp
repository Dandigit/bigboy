#include <bigboy/MMU/MMU.h>

#include <algorithm>
#include <iostream>

MMU::MMU() {
    registerDevice(m_internal);
}

MMU::MMU(std::initializer_list<std::reference_wrapper<MemoryDevice>> devices) : MMU{} {
    for (auto& device : devices) {
        registerDevice(device);
    }
}

uint8_t MMU::readByte(uint16_t address) const {
    if (const MemoryDevice* device = getDevice(address)) {
        return device->readByte(address);
    }

    std::cerr << "No memory device registered for address: " << address << '\n';
    return 0xFF; // Return bogus
}

void MMU::writeByte(uint16_t address, uint8_t value) {
    if (MemoryDevice* device = getDevice(address)) {
        return device->writeByte(address, value);
    }

    std::cerr << "No memory device registered for address: " << address << '\n';
    // Do nothing.
}

uint16_t MMU::readWord(uint16_t address) const {
    uint8_t lower = readByte(address);
    uint8_t higher = readByte(address + 1);
    return (higher << 8u) | lower;
}

void MMU::writeWord(uint16_t address, uint16_t value) {
    writeByte(address, (value & 0xFFu));
    writeByte(address, ((value >> 8u) & 0xFF));
}

void MMU::registerDevice(MemoryDevice &device) {
    for (const AddressSpace& addressSpace : device.addressSpaces()) {
        reserveAddressSpace(device, addressSpace);
    }
}

void MMU::reserveAddressSpace(MemoryDevice &device, AddressSpace addressSpace) {
    uint16_t i = addressSpace.start;
    do {
        m_devices[i] = &device;
    } while (i++ < addressSpace.end);
}

void MMU::reset() {
    //m_devices.fill(nullptr);
    m_internal.reset();
    registerDevice(m_internal);
}

MemoryDevice* MMU::getDevice(uint16_t address) {
    if (MemoryDevice* device = m_devices[address]) {
        return device;
    }

    return nullptr;
}

const MemoryDevice* MMU::getDevice(uint16_t address) const {
    if (const MemoryDevice* device = m_devices[address]) {
        return device;
    }

    return nullptr;
}
