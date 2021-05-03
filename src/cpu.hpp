#include <cstdint>

#include "memory.hpp"
#include "display.hpp"

class CPU{
private:
    Memory memory;   // Memory Instance
    Display display; // Display Instance

    void execute(uint16_t instruction);

public:
    // -----REGISTERS-----
    // General Purpose Registers
    uint8_t V[0xF];
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
    CPU();
    void step();
};