#include "display.hpp"

Display::Display(Memory* memory){
    Display::clear(); // Clear Display Before First Use
    Display::memory = memory;
}

void Display::clear(){
    for(unsigned int i = 0; i < 32; i++){
        for(unsigned int j = 0; j < 64; j++){
            Display::pixelData[i][j] = 0x0;
        }
    }
}

bool Display::drawSprite(uint16_t I, uint8_t Vx, uint8_t Vy, uint8_t n){
    bool collided = false;

    // Draw Sprite
    for(unsigned int i = 0; i < n; i++){
        // Fetch Byte From Memory
        uint8_t spriteByte = Display::memory -> map[I + i];

        for(unsigned int j = 0; j < 8; j++){
            bool pixelState = Display::pixelData[Vy + i][Vx + j];
            Display::pixelData[Vy + i][Vx + j] ^= (spriteByte >> (7 - j)) & 0b1;

            if(Display::pixelData[Vy + i][Vx + j] != pixelState && pixelState == true){
                collided = true;
            }
        }
    }

    return collided;
}