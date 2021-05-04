#include "emulator.hpp"

Emulator::Emulator(unsigned int speed){
    Emulator::memory = new Memory;
    Emulator::keyboard = new Keyboard;
    Emulator::cpu = new CPU(Emulator::memory, Emulator::display, Emulator::keyboard);

    Emulator::exit = false;

    Emulator::speed = speed;
}

bool Emulator::loadGame(const char* path){
    std::ifstream gameStream;
    gameStream.open(path, std::ios::binary);

    if(gameStream.good()){
        gameStream.seekg(0, std::ios::end);
        int end = gameStream.tellg();
        
        gameStream.seekg(0, std::ios::beg);        
        int beg = gameStream.tellg();

        int size = end - beg;

        gameStream.read(&Emulator::memory -> map[0x200], size);

        gameStream.close();
        return true;
    
    }else{
        gameStream.close();
        return false;
    }
}