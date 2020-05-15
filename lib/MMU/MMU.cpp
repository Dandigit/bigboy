#include <bigboy/MMU/MMU.h>

#include <algorithm>
#include <string>

MMU::MMU(std::initializer_list<std::reference_wrapper<MemoryDevice>> devices) :
        m_devices{std::move(devices)} {
    registerDevice(m_internal);
}

uint8_t MMU::readByte(uint16_t address) const {
    return getDevice(address).readByte(address);
}

void MMU::writeByte(uint16_t address, uint8_t value) {
    return getDevice(address).writeByte(address, value);
}

void MMU::registerDevice(MemoryDevice &device) {
    m_devices.emplace_back(device);
}

void MMU::deregisterDevice(MemoryDevice& deviceR) {
    auto device = std::find_if(
            m_devices.begin(), m_devices.end(), [&](auto device){ return &deviceR == &device.get(); });

    if (device == m_devices.end()) {
        throw std::runtime_error{"[fatal]: Cannot deregister already deregistered memory device"};
    }

    m_devices.erase(device);
}

void MMU::reset() {
    m_devices.erase(m_devices.begin() + 1);
}

MemoryDevice& MMU::getDevice(uint16_t address) {
    auto device = std::find_if(
            m_devices.begin(), m_devices.end(),
            [address](auto device){ return device.get().doesAddress(address); });

    if (device == m_devices.end()) {
        throw std::runtime_error{"[fatal]: No memory device registered for address: " +
                std::to_string(address)};
    }

    return *device;
}

const MemoryDevice& MMU::getDevice(uint16_t address) const {
    const auto device = std::find_if(
            m_devices.begin(), m_devices.end(),
            [address](auto device){ return device.get().doesAddress(address); });

    if (device == m_devices.end()) {
        throw std::runtime_error{"[fatal]: No memory device registered for address: " +
                                 std::to_string(address)};
    }

    return *device;
}