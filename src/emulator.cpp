#include "emulator.hpp"

Emulator::Emulator(unsigned int speed){
    Emulator::memory = new Memory;
    Emulator::keyboard = new Keyboard;
    Emulator::cpu = new CPU(Emulator::memory, Emulator::display);

    Emulator::speed = speed;
}