#include "cpu.hpp"

CPU::CPU(){
    CPU::PC = 0x200; // Start Program Counter At 0x200
}

void CPU::execute(uint16_t instruction){
    // IGNORED BY MOST INTERPRETERS
    // if(instruction >> 12 == 0x0){ 
    //    ;
    // }

    // CLS - Clear The Screen
    if(instruction == 0x00E0){
        CPU::display.clear();
    }

    // RET - Return From Subroutine
    if(instruction == 0x00EE){
         CPU::PC = CPU::stack[CPU::SP];
         CPU::SP -= 1;
    }

    // JP - Jump To Address
    if((instruction & 0xF000) == 0x1000){
        CPU::PC = instruction & 0x0FFF;
    }

    // CALL - Call Subroutine
    if((instruction & 0xF000) == 0x2000){
        CPU::SP++;
        CPU::stack[CPU::SP] = CPU::PC;
        CPU::PC = instruction & 0x0FFF;
    }

    // SE - Skip next instruction if Vx = kk (3xkk)
    if((instruction & 0xF000) == 0x3000){
        if(CPU::V[(instruction & 0x0F00) >> 12] == (instruction & 0x00FF)){
            CPU::PC += 4;
        }
    }

    // SNE - Skip next instruction if Vx != kk (4xkk)
    if((instruction & 0xF000) == 0x4000){
        if(!(CPU::V[(instruction & 0x0F00) >> 12] == (instruction & 0x00FF))){
            CPU::PC += 4;
        }
    }

    // SE - Skip Next Instruction if Vx = Vy (5xy0)
    if((instruction & 0xF000) == 0x5000){
        if((CPU::V[(instruction & 0x0F00)] >> 12) == (CPU::V[(instruction & 0x00F0)] >> 8)){
            CPU::PC += 4;
        }
    }

    // LD - Set Vx == kk (6xkk)
    if((instruction & 0xF000) == 0x6000){
        CPU::V[(instruction & 0x0F00) >> 12] = (instruction & 0x00FF);
    }
}

void CPU::step(){
    CPU::PC += 2; // Increment Program Counter

    if(CPU::DT > 0){
        CPU::DT -= 1; // Decrement Delay Timer If Active
    }

    if(CPU::ST > 0){
        CPU::ST -= 1; // Decrement Sound Timer If Active
    }

    // Fetch Instruction
    char signByte   = CPU::memory.map[CPU::PC];     // Significant Byte
    char insignByte = CPU::memory.map[CPU::PC + 1]; // Insignificant Byte

    uint16_t instruction = (signByte << 8) + insignByte; // Combine Bytes To 2 Byte Instruction
    CPU::execute(instruction); // Execute Instruction
}