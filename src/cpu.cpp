#include "cpu.hpp"

CPU::CPU(Memory* memory, Display* display, Keyboard* keyboard){
    CPU::PC = 0x200; // Start Program Counter At 0x200

    CPU::memory   = memory;
    CPU::display  = display;
    CPU::keyboard = keyboard;

    CPU::reset();   // Reset CPU Before First Use
}

void CPU::execute(uint16_t instruction){
    // IGNORED BY MOST INTERPRETERS
    // if(instruction >> 8 == 0x0){ 
    //    ;
    // }

    // CLS - Clear The Screen
    if(instruction == 0x00E0){
        CPU::display -> clear();
        CPU::PC += 2; // Increment Program Counter
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
        CPU::PC = (instruction & 0x0FFF);
    }

    // SE - Skip next instruction if Vx = kk (3xkk)
    if((instruction & 0xF000) == 0x3000){
        if(CPU::V[(instruction & 0x0F00) >> 8] == (instruction & 0x00FF)){
            CPU::PC += 2;
        }

        CPU::PC += 2;
    }

    // SNE - Skip next instruction if Vx != kk (4xkk)
    if((instruction & 0xF000) == 0x4000){
        if(!(CPU::V[(instruction & 0x0F00) >> 8] == (instruction & 0x00FF))){
            CPU::PC += 2;
        }

        CPU::PC += 2;
    }

    // SE - Skip Next Instruction if Vx = Vy (5xy0)
    if((instruction & 0xF000) == 0x5000){
        if((CPU::V[(instruction & 0x0F00)] >> 8) == (CPU::V[(instruction & 0x00F0)] >> 4)){
            CPU::PC += 4;
        }
    }

    // LD - Set Vx == kk (6xkk)
    if((instruction & 0xF000) == 0x6000){
        CPU::V[(instruction & 0x0F00) >> 8] = (instruction & 0x00FF);
        CPU::PC += 2; // Increment Program Counter
    }

    // ADD - Set Vx = Vx + kk (7xkk)
    if((instruction & 0xF000) == 0x7000){
        CPU::V[(instruction & 0x0F00) >> 8] += (instruction & 0x00FF);
        CPU::PC += 2; // Increment Program Counter
    }

    if((instruction & 0xF000) == 0x8000){
        // LD - Set Vx = Vy (8xy0)
        if((instruction & 0x000F) == 0x0000){
            CPU::V[(instruction & 0x0F00) >> 8] = CPU::V[(instruction & 0x00F0 >> 4)];
            CPU::PC += 2; // Increment Program Counter
        }

        // OR - Set Vx = Vx OR Vy (8xy1)
        if((instruction & 0x000F) == 0x0001){
            CPU::V[(instruction & 0x0F00) >> 8] = CPU::V[(instruction & 0x0F00) >> 8] | CPU::V[(instruction & 0x00F0) >> 4];
            CPU::PC += 2; // Increment Program Counter
        }

        // AND - Set Vx = Vx AND Vy (8xy2)
        if((instruction & 0x000F) == 0x0002){
            CPU::V[(instruction & 0x0F00) >> 8] = CPU::V[(instruction & 0x0F00) >> 8] & CPU::V[(instruction & 0x00F0) >> 4];
            CPU::PC += 2; // Increment Program Counter
        }

        // XOR - Set Vx = Vx XOR Vy (8xy3)
        if((instruction & 0x000F) == 0x0003){
            CPU::V[(instruction & 0x0F00) >> 8] = CPU::V[(instruction & 0x0F00) >> 8] ^ CPU::V[(instruction & 0x00F0) >> 4];
            CPU::PC += 2; // Increment Program Counter
        }

        // ADD - Set Vx = Vx + Vy, set VF = carry (8xy4)
        if((instruction & 0x000F) == 0x0004){
            uint16_t sum = CPU::V[(instruction & 0x0F00) >> 8] + CPU::V[(instruction & 0x00F0) >> 4];

            CPU::V[(instruction & 0x0F00) >> 8] = sum;

            if(sum > 0xFF){
                CPU::V[0xF] = 0x1;
            }else{
                CPU::V[0xF] = 0x0;
            }

            CPU::PC += 2; // Increment Program Counter
        }

        // SUB - Set Vx = Vx - Vy, set VF = NOT borrow (8xy5)
        if((instruction & 0x000F) == 0x0005){
            if(CPU::V[(instruction & 0x0F00) >> 8] > V[(instruction & 0x00F0) >> 4]){
                CPU::V[0XF] = 0x1;
            }else{
                CPU::V[0xF] = 0x0;
            }

            CPU::V[(instruction & 0x0F00) >> 8] -= CPU::V[(instruction & 0x00F0) >> 4];

            CPU::PC += 2; // Increment Program Counter
        }

        // SHR - Set Vx = Vx SHR 1 (8xy6)
        if((instruction & 0x000F) == 0x0006){
            if((CPU::V[(instruction & 0x0F00) >> 8] & 0x01) == 0x1){
                CPU::V[0xF] = 0x1;
            }else{
                CPU::V[0xF] = 0x0;
            }

            CPU::V[(instruction & 0x0F00) >> 8] /= 2;

            CPU::PC += 2; // Increment Program Counter
        }

        // SUBN - Set Vx = Vy - Vx, set VF = NOT borrow (8xy7)
        if((instruction & 0x000F) == 0x0007){
            if(CPU::V[(instruction & 0x0F00) >> 8] < V[(instruction & 0x00F0) >> 4]){
                CPU::V[0XF] = 0x1;
            }else{
                CPU::V[0xF] = 0x0;
            }

            CPU::V[(instruction & 0x0F00) >> 8] = CPU::V[(instruction & 0x00F0) >> 4] - CPU::V[(instruction & 0x0F00) >> 8];

            CPU::PC += 2; // Increment Program Counter
        }

        // SHL - Set Vx = Vx SHL 1 (8xyE)
        if((instruction & 0x000F) == 0x000E){
            if(((CPU::V[(instruction & 0x0F00) >> 8] & 0b1000000000000000) >> 16) == 0x01){
                CPU::V[0xF] = 0x01;
            }else{
                CPU::V[0xF] = 0x00;
            }

            CPU::V[(instruction & 0x0F00) >> 8] *= 2;

            CPU::PC += 2; // Increment Program Counter
        }
    }

    // SNE - Skip next instruction if Vx != Vy (9xy0)
    if((instruction & 0xF000) == 0x9000){
        if(!(CPU::V[(instruction & 0x0F00) >> 8] == CPU::V[(instruction & 0x00F0) >> 4])){
            CPU::PC += 2;
        }
        CPU::PC += 2; // Increment Program Counter
    }

    // LD - Set I = nnn (Annn)
    if((instruction & 0xF000) == 0xA000){
        CPU::I = (instruction & 0x0FFF);
        CPU::PC += 2; // Increment Program Counter
    }

    // JP - Jump to location nnn + V0 (Bnnn)
    if((instruction & 0xF000) == 0xB000){
        CPU::PC = (instruction & 0x0FFF) + CPU::V[0x0];
    }

    // RND - Set Set Vx = random byte AND kk (Cxkk)
    if((instruction & 0xF000) == 0xC000){
        uint8_t random = rand() % 256;
        CPU::V[(instruction & 0x0F00) >> 8] = random & (instruction & 0x00FF);
        CPU::PC += 2; // Increment Program Counter
    }

    // DRW - Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision (Dxyn)
    if((instruction & 0xF000) == 0xD000){
        uint8_t Vx = CPU::V[(instruction & 0x0F00) >> 8];
        uint8_t Vy = CPU::V[(instruction & 0x00F0) >> 4];

        CPU::display -> drawSprite(CPU::I, Vx, Vy, instruction & 0x000F);
        CPU::PC += 2; // Increment Program Counter
    }

    if((instruction & 0xF000) == 0xE000){
        // SKP - Skip next instruction if key with the value of Vx is pressed (Ex9E)
        if((instruction & 0x00FF) == 0x009E){
            if(CPU::keyboard -> isPressed(CPU::V[(instruction & 0x0F00) >> 8])){
                CPU::PC += 4;
            }
        }

        // SKNP - Skip next instruction if key with the value of Vx is not pressed (ExA1)
        if((instruction & 0x00FF) == 0x00A1){
            if(!(CPU::keyboard -> isPressed(CPU::V[(instruction & 0x0F00) >> 8]))){
                CPU::PC += 4;
            }
        }
    }

    if((instruction & 0xF000) == 0xF000){
        // LD - Set Vx = delay timer value (Fx07)
        if((instruction & 0x00FF) == 0x0007){
            CPU::V[(instruction & 0x0F00) >> 8] = CPU::DT;
            CPU::PC += 2; // Increment Program Counter
        }
        
        // LD - Wait for a key press, store the value of the key in Vx (Fx0A)
        if((instruction & 0x00FF) == 0x000A){
            uint8_t pressedButton = CPU::keyboard -> anyPressed();
            if(!(pressedButton > 0xf)){
                CPU::V[(instruction & 0x0F00) >> 8] = pressedButton;
                CPU::PC += 2; // Increment Program Counter
            }
        }

        // LD - Set delay timer = Vx (Fx15)
        if((instruction & 0x00FF) == 0x0015){
            CPU::DT = CPU::V[(instruction & 0x0F00) >> 8];
            CPU::PC += 2; // Increment Program Counter
        }

        // LD - Set sound timer = Vx (Fx18)
        if((instruction & 0x00FF) == 0x0018){
            CPU::ST = CPU::V[(instruction & 0x0F00) >> 8];
            CPU::PC += 2; // Increment Program Counter
        }

        // ADD - Set I = I + Vx (Fx1E)
        if((instruction & 0x00FF) == 0x001E){
            CPU::I += CPU::V[(instruction & 0X0F00) >> 8];
            CPU::PC += 2; // Increment Program Counter
        }

        // LD - Set I = location of sprite for digit Vx (Fx29)
        if((instruction & 0x00FF) == 0x0029){
            CPU::I = (CPU::V[(instruction & 0x0F00) >> 8] * 5);
            CPU::PC += 2; // Increment Program Counter
        }

        // LD - Store BCD representation of Vx in memory locations I, I+1, and I+2 (Fx33)
        if((instruction & 0x00FF) == 0x0033){
            // TO BE ADDED
            // CPU::PC += 2; // Increment Program Counter
        }

        // LD - Store registers V0 through Vx in memory starting at location I (Fx55)
        if((instruction & 0x00FF) == 0x0055){
            // TO BE ADDED
            // CPU::PC += 2; // Increment Program Counter
        }

        // LD - Read registers V0 through Vx from memory starting at location I (Fx65)
        if((instruction & 0x00FF) == 0x0065){
            // TO BE ADDED
            // CPU::PC += 2; // Increment Program Counter
        }
    }
}

void CPU::reset(){
    // -----RESET REGISTERS-----
    // Reset General Purpose Registers
    for(unsigned int i = 0; i < 16; i++){
        V[i] = 0x00;
    }
    // Reset Register I
    CPU::I = 0x0000;
    // Reset Special Purpose Registers
    CPU::DT = 0x00;
    CPU::ST = 0x00;
    // Reset Pseudo Registers
    CPU::PC = 0x0200;
    CPU::SP = 0x00;
    // Reset Stack
    for(unsigned int i = 0; i < 16; i++){
        CPU::stack[i] = 0x0000;
    }
}

void CPU::step(){
    // Fetch Instruction
    uint8_t signByte   = CPU::memory -> map[CPU::PC];     // Significant Byte
    uint8_t insignByte = CPU::memory -> map[CPU::PC + 1]; // Insignificant Byte

    uint16_t instruction = (((uint16_t) signByte) << 8) + insignByte; // Combine Bytes To 2 Byte Instruction

    // Print PC And Instruction 
    std::cout << "PC: " << CPU::PC - 0x200 << " instruction: " << std::hex << instruction << std::endl;
    CPU::execute(instruction); // Execute Instruction
}

void CPU::decrementTimers(){
    if(CPU::DT > 0){
        CPU::DT -= 1; // Decrement Delay Timer If Active
    }

    if(CPU::ST > 0){
        CPU::ST -= 1; // Decrement Sound Timer If Active
    }
}