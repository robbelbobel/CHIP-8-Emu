#ifndef display_hpp
#define display_hpp

#include <cstdint>
#include <iostream>

#include "memory.hpp"

class Display{
private:
    Memory* memory;
public:
    // Screen Size 64x32 (w: 64 (8 * 8 bytes), h: 32)
    uint64_t pixelData[32];

    // Public Class Functions
    Display(Memory* memory);
    void clear();
    bool drawSprite(uint16_t I, uint8_t Vx, uint8_t Vy, char n);
};

#endif