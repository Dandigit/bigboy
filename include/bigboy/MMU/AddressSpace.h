#ifndef BIGBOY_ADDRESSSPACE_H
#define BIGBOY_ADDRESSSPACE_H

struct AddressSpace {
    uint16_t start;
    uint16_t end;

    AddressSpace(uint16_t start, uint16_t end) : start{start}, end{end} {}
    explicit AddressSpace(uint16_t addr) : start{addr}, end{addr} {}
};

#endif //BIGBOY_ADDRESSSPACE_H
