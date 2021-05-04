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
    int windowWidth  = 0;
    int windowHeight = 0;
    SDL_GetWindowSize(Emulator::window, &windowWidth, &windowHeight);

    // Get Window Surface
    SDL_Surface* windowSurface = SDL_GetWindowSurface(Emulator::window);

    SDL_Rect pixel;
    pixel.w = windowWidth / 64;
    pixel.h = windowHeight / 32;
    
    for(unsigned int i = 0; i < 32; i++){
        pixel.y = i * pixel.h;
        for(unsigned int j = 0; j < 64; j++){
            pixel.x = j * pixel.w;
            if(((Emulator::display -> pixelData[i] & (0b1 << j)) >> j) == 1){
                SDL_FillRect(windowSurface, &pixel, SDL_MapRGBA(windowSurface -> format, Emulator::pixelLit.r, Emulator::pixelLit.g, Emulator::pixelLit.b, Emulator::pixelLit.a));
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

    // Print Display Pixel Data
    for (unsigned int i = 0; i < 32; i++) {
        for (unsigned int j = 0; j < 64; j++) {
            char byte = (Emulator::display -> pixelData[i] >> j) & 1;
            printf("%u", byte);
        }

        std::cout << std::endl;
    }
    
    std::cout << std::endl << std::endl;
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
        char gameData[size];
        gameStream.read(gameData, size);

        // Load Game Data Into Memory
        for(unsigned int i = 0; i < size; i++){
            Emulator::memory -> map[0x200 + i] = (uint8_t) gameData[i];
        }

        gameStream.close();

        Emulator::cpu     -> reset();   // Reset Processor
        Emulator::display -> clear();   // Clear Display

        return true;
    
    }else{
        gameStream.close();
        return false;
    }
}