#include "emulator.hpp"

Emulator::Emulator(unsigned int speed){
    Emulator::memory = new Memory;
    Emulator::keyboard = new Keyboard;
    Emulator::cpu = new CPU(Emulator::memory, Emulator::display, Emulator::keyboard);

    Emulator::speed = speed;
}

bool Emulator::loadGame(const char* path){
    std::ifstream gameStream;
    gameStream.open(path, std::ios::binary);

    if(gameStream.good()){
        gameStream.

        gameStream.seekg(0);
        gameStream.read(Emulator::memory, );
    }
    
    return false;
}