#include <iostream>
#include <fstream>

#include "cpu.hpp"

class Emulator{
    private:
        CPU* cpu;           // CPU Instance
        Memory* memory;     // Memory Instance
        Display* display;   // Display Instance
        
        unsigned int speed;

        bool exit;

    public:
        // Public Class Variables
        Keyboard* keyboard; // Keyboard Instance

        // Public Class Functions
        Emulator(unsigned int speed);

        bool loadGame(const char* path);
};