#ifndef memory_hpp
#define memory_hpp

#include <fstream>

class Memory{
public:
    Memory();

    char map[4096]; // Memory Map (Array) (Code starts at 0x200)
    
};

#endif