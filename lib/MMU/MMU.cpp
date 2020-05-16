#include <bigboy/MMU/MMU.h>

#include <algorithm>
#include <string>

MMU::MMU() {
    registerDevice(m_internal);
}

MMU::MMU(std::initializer_list<std::reference_wrapper<MemoryDevice>> devices) : MMU{} {
    for (auto& device : devices) {
        registerDevice(device);
    }
}

uint8_t MMU::readByte(uint16_t address) const {
    return getDevice(address).readByte(address);
}

void MMU::writeByte(uint16_t address, uint8_t value) {
    return getDevice(address).writeByte(address, value);
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
    } while (++i < addressSpace.end);
}

void MMU::reset() {
    m_devices.fill(nullptr);
    registerDevice(m_internal);
}

MemoryDevice& MMU::getDevice(uint16_t address) {
    if (MemoryDevice* device = m_devices[address]) {
        return *device;
    }

    throw std::runtime_error{"No memory device registered for address: " + std::to_string(address)};
}

const MemoryDevice& MMU::getDevice(uint16_t address) const {
    if (const MemoryDevice* device = m_devices[address]) {
        return *device;
    }

    throw std::runtime_error{"No memory device registered for address: " + std::to_string(address)};
}
