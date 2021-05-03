#include "cpu.hpp"

class Emulator{
    private:
        CPU* cpu;           // CPU Instance
        Memory* memory;     // Memory Instance
        Display* display;   // Display Instance
        Keyboard* keyboard; // Keyboard Instance
        
        unsigned int speed;

    public:
        Emulator(unsigned int speed);
};