#include <cstdint>

class Display{
public:
    // Screen Size 64x32

    void clear();
    bool drawSprite(uint16_t I, uint8_t Vx, uint8_t Vy, char n);
};