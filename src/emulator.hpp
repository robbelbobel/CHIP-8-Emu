#ifndef emulator_hpp
#define emulator_hpp

#include <iostream>
#include <fstream>
#include <SDL.h>

#include "cpu.hpp"

class Emulator{
    private:
        CPU* cpu;           // CPU Instance
        Memory* memory;     // Memory Instance
        Display* display;   // Display Instance
        
        SDL_Window* window;
        
        unsigned int speed;

        bool exit;

        SDL_Color pixelLit;
        SDL_Color pixelDim;

        uint32_t stepTimeCounter;
        uint32_t timerTimeCounter;

        // Private Class Functions
        void draw();

    public:
        // Public Class Variables
        Keyboard* keyboard; // Keyboard Instance

        // Public Class Functions
        Emulator(unsigned int speed, SDL_Window* window);
        void run(uint32_t dTime);
        bool loadGame(const char* path);
};

#endif