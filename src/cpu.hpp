#include <cstdint>
#include <random>

#include "memory.hpp"
#include "display.hpp"
#include "keyboard.hpp"

class CPU{
private:
    Memory* memory;     // Memory Pointer
    Display* display;   // Display Pointer
    Keyboard* keyboard; // Keyboard Pointer

    void execute(uint16_t instruction);

public:
    // -----REGISTERS-----
    // General Purpose Registers
    uint8_t V[16];
    // 16 Bit Register I
    uint16_t I;
    // Special Purpose Registers (Get Decremented At 60hz If Not 0)
    uint8_t DT; // Delay Timer
    uint8_t ST; // Sound Timer
    // Pseudo Registers (Not Accessible By Programs)
    uint16_t PC; // Program Counter (Used To Store Currently Executing Address)
    uint8_t SP;  // Points To Topmost Level Of The Stack

    uint16_t stack[16]; // Stores the address that the interpreter should return to after completing a subroutine (max. 16 nested subroutines)

    // Public Functions
    CPU(Memory* memory, Display* display, Keyboard* keyboard);

    void step();
    void decrementTimers();
};