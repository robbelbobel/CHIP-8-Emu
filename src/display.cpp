#include "display.hpp"

Display::Display(Memory* memory){
    Display::clear(); // Clear Display Before First Use
    Display::memory = memory;
}

void Display::clear(){
    for(unsigned int i = 0; i < 32; i++){
        Display::pixelData[i] = 0;
    }
}

bool Display::drawSprite(uint16_t I, uint8_t Vx, uint8_t Vy, char n){
    // Draw Sprite
    for(unsigned int i = 0; i < n; i++){
        Display::pixelData[Vy + i] = ((Display::memory -> map[I + i]) << (56 - Vx));
    }
    return false;
}