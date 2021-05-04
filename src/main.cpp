#include <iostream>
#include <SDL.h>

#include "emulator.hpp"

#define EMULATION_SPEED 500 // Emulation Speed In Hz

// Global Variables
char keyMap[16] = {SDL_SCANCODE_X, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_A, SDL_SCANCODE_Z, SDL_SCANCODE_E, SDL_SCANCODE_Q, SDL_SCANCODE_S, SDL_SCANCODE_D, SDLK_w, SDL_SCANCODE_C, SDL_SCANCODE_4, SDL_SCANCODE_R, SDL_SCANCODE_F, SDL_SCANCODE_V};

// Function Declarations
void getInput(bool &running, Emulator &emulator, SDL_Window* window);

int main(){
    // -----INITIALIZE-----
    // SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cerr << "Failed to initialize SDL!" << std::endl;
        return -1;
    }

    // SDL-Window
    SDL_Window* window = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 500, SDL_WINDOW_SHOWN);

    if(window == nullptr){
        std::cerr << "Failed to create SDL window!" << std::endl;
        return -1;
    }

    // Window-Surface
    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);

    if(windowSurface == nullptr){
        std::cerr << "Couldn't get window surface!" << std::endl;
        return -1;
    }

    // -----VARIABLES-----
    Emulator emulator(EMULATION_SPEED);
    bool running = true;

    while(running){
        // Clear Screen
        SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface -> format, 0x00, 0x00, 0x00));

        // Get Input
        getInput(running, emulator, window);

        // Update Window
        SDL_UpdateWindowSurface(window);
    }

    // Shut Emulator Down
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void getInput(bool &running, Emulator &emulator, SDL_Window* window){
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0){
        if(e.type == SDL_QUIT){
            running = false;
        }

        if(e.type == SDL_KEYDOWN){
            switch(e.key.keysym.sym){
                case SDLK_ESCAPE:
                    running = false;
                    break;
            }
        }

        if(e.type == SDL_DROPFILE){
            if(!emulator.loadGame(e.drop.file)){
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't load your CHIP-8 ROM!", window);
            }
        }
    }

    // Get Key States
    const uint8_t *state = SDL_GetKeyboardState(NULL);

    // Set Keyboard Keys
    for(unsigned int i = 0; i < 16; i++){
        emulator.keyboard -> pressedKeys[i] = state[keyMap[i]];
    }
}