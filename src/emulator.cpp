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

    // Clear Screen
    SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface -> format, 0x00, 0x00, 0x00));

    // Calculate Pixel Size
    SDL_Rect pixel;
    pixel.w = windowWidth / 64;
    pixel.h = windowHeight / 32;

    if((windowWidth / 64) >= (windowHeight / 32)){
        pixel.h = windowHeight / 32;
        pixel.w = pixel.h;
    }else{
        pixel.w = windowWidth / 64;
        pixel.h = pixel.w;
    }

    for(int i = 0; i < 32; i++){
        pixel.y = i * pixel.h;
        for(int j = 0; j < 64; j++){
            pixel.x = j * pixel.w;

            if(Emulator::display -> pixelData[i][j] == 0x1){
                SDL_FillRect(windowSurface, &pixel, SDL_MapRGB(windowSurface -> format, Emulator::pixelLit.r, Emulator::pixelLit.g, Emulator::pixelLit.b));
            }else{
                SDL_FillRect(windowSurface, &pixel, SDL_MapRGB(windowSurface -> format, Emulator::pixelDim.r, Emulator::pixelDim.g, Emulator::pixelDim.b));
            }
        }
    }

    // Update Window
    SDL_UpdateWindowSurface(window);
}

void Emulator::run(uint32_t DTime){
    Emulator::stepTimeCounter  += DTime;
    Emulator::timerTimeCounter += DTime;

    // Check StepTimeCounter
    if(Emulator::stepTimeCounter >= (1000.0f / Emulator::speed)){
        Emulator::cpu -> step();
        Emulator::stepTimeCounter = 0;
    }

    // Check timerTimeCounter (Should Be Exectuted At 60Hz)
    if(Emulator::timerTimeCounter >= (17)){
        Emulator::draw();   // Draw Screen 60 Times Per Second

        Emulator::cpu -> decrementTimers(); // Decrement CPU Timers
        Emulator::timerTimeCounter = 0;     // Reset TimerTimeCounter
    }

    // Play Sound If Sound Timer Is Not Zero
    if(Emulator::cpu -> ST != 0){
        std::cout << "BEEP" << std::endl;
    }
}

bool Emulator::loadGame(const char* path){
    std::ifstream gameStream;
    gameStream.open(path, std::ios::binary);

    if(gameStream.good()){
        gameStream.seekg(0, std::ios::end);
        int end = gameStream.tellg();
        
        gameStream.seekg(0, std::ios::beg);        
        int beg = gameStream.tellg();

        int size = end - beg; // Calculate Game File Length

        char gameData[size];

        // Read Game Data
        gameStream.read(gameData, size);

        gameStream.close(); // Close Game Data Stream
        
        // Load Game Data Into Memory
        for(unsigned int i = 0; i < size; i++){
            Emulator::memory -> map[0x200 + i] = (uint8_t) gameData[i];
        }

        Emulator::cpu     -> reset();   // Reset Processor
        Emulator::display -> clear();   // Clear Display
        return true;
    
    }else{
        gameStream.close();
        return false;
    }
}