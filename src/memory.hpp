#ifndef memory_hpp
#define memory_hpp

#include <vector>

class Memory{
public:
    Memory();

    uint8_t map[4096]; // Memory Map (Array) (Code starts at 0x200)
};

#endif