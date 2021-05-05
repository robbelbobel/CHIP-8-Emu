#include "emulator.hpp"

Emulator::Emulator(unsigned int speed, SDL_Window* window){
    Emulator::memory = new Memory;
    Emulator::keyboard = new Keyboard;
    Emulator::display = new Display(Emulator::memory);
    Emulator::cpu = new CPU(Emulator::memory, Emulator::display, Emulator::keyboard);

    Emulator::window = window;

    Emulator::exit = false;

    Emulator::speed = speed;

    Emulator::pixelLit.r = 0xff;
    Emulator::pixelLit.g = 0xff;
    Emulator::pixelLit.b = 0xff;
    Emulator::pixelLit.a = 0xff;

    Emulator::pixelDim.r = 0x00;
    Emulator::pixelDim.g = 0x00;
    Emulator::pixelDim.b = 0x00;
    Emulator::pixelDim.a = 0xff;

    // Set Timers to 0
    Emulator::stepTimeCounter  = 0;
    Emulator::timerTimeCounter = 0;
}

void Emulator::draw(){
    // Get Window Dimensions
    int windowWidth;
    int windowHeight;
    SDL_GetWindowSize(Emulator::window, &windowWidth, &windowHeight);

    // Get Window Surface
    SDL_Surface* windowSurface = SDL_GetWindowSurface(Emulator::window);

    SDL_Rect pixel;
    pixel.w = windowWidth / 64;
    pixel.h = windowHeight / 32;
    
    for(int i = 0; i < 32; i++){
        pixel.y = i * pixel.h;
        for(int j = 64; j >= 0; j--){
            pixel.x = (64 - j) * pixel.w;

            if(((Emulator::display -> pixelData[i] & (((uint64_t) 0b1) << j)) >> j)){
                SDL_FillRect(windowSurface, &pixel, SDL_MapRGBA(windowSurface -> format, Emulator::pixelLit.r, Emulator::pixelLit.g, Emulator::pixelLit.b, Emulator::pixelLit.a));
            }else{
                SDL_FillRect(windowSurface, &pixel, SDL_MapRGBA(windowSurface -> format, Emulator::pixelDim.r, Emulator::pixelDim.g, Emulator::pixelDim.b, Emulator::pixelDim.a));
            }
        }
    }
}

void Emulator::run(uint32_t DTime){
    Emulator::stepTimeCounter  += DTime;
    Emulator::timerTimeCounter += DTime;

    // Check StepTimeCounter
    if(Emulator::stepTimeCounter >= (1000 / Emulator::speed)){
        Emulator::cpu -> step();
        Emulator::stepTimeCounter = 0;
    }

    // Check timerTimeCounter (Should Be Exectuted At 60Hz)
    if(Emulator::timerTimeCounter >= (17)){
        Emulator::cpu -> decrementTimers(); // Decrement CPU Timers
        Emulator::timerTimeCounter = 0;
    }

    Emulator::draw(); // Draw Display To Window
}

bool Emulator::loadGame(const char* path){
    std::ifstream gameStream;
    gameStream.open(path, std::ios::binary);

    if(gameStream.good()){
        gameStream.seekg(0, std::ios::end);
        int end = gameStream.tellg();
        
        gameStream.seekg(0, std::ios::beg);        
        int beg = gameStream.tellg();

        int size = end - beg;

        // Read Game Data
        gameStream.read(&Emulator::memory -> map[0x200], size);

        gameStream.close();

        Emulator::cpu     -> reset();   // Reset Processor
        Emulator::display -> clear();   // Clear Display

        std::cout << "Memory Dump: " << std::endl;
        for(unsigned int i = 0; i < 4096; i++){
            printf("%x", Emulator::memory -> map[i]);
        }

        std::cout << std::endl;

        return true;
    
    }else{
        gameStream.close();
        return false;
    }
}