#ifndef keyboard_hpp
#define keyboard_hpp

#include <cstdint>
#include <iostream>

class Keyboard{
public:
    // Public Variables
    bool pressedKeys[16];

    // Public Functions
    bool isPressed(uint8_t Vx);
    uint8_t anyPressed();
};

#endif