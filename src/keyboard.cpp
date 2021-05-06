#include "keyboard.hpp"

bool Keyboard::isPressed(uint8_t Vx){
    // Check if Vx is pressed
    if(Keyboard::pressedKeys[Vx]){
        return true;
    }else{
        return false;
    }
}

uint8_t Keyboard::anyPressed(){
    for(unsigned int i = 0; i < 16; i++){
        if(Keyboard::pressedKeys[i]){
            return i;
        }
    }

    return 0x10;
}