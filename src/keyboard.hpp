#include <cstdint>

class Keyboard{
public:
    bool isPressed(uint8_t Vx);
    uint8_t anyPressed();
};