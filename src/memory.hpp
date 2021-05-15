#ifndef memory_hpp
#define memory_hpp

#include <fstream>

class Memory{
public:
    Memory();

    uint8_t map[4096]; // Memory Map (Array) (Code starts at 0x200)
    void dump();
};

#endif