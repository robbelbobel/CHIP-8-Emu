#include <fstream>

#include "cpu.hpp"

class Emulator{
    private:
        CPU* cpu;           // CPU Instance
        Memory* memory;     // Memory Instance
        Display* display;   // Display Instance
        Keyboard* keyboard; // Keyboard Instance
        
        unsigned int speed;

        bool loadGame(const char* path);

    public:
        Emulator(unsigned int speed);
};