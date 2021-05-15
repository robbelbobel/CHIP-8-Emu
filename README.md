# CHIP-8 Emu

CHIP-8 Emu is an interpreter for the CHIP-8.
The scope for this project is to learn about emulators and computer interfacing in general.
It's not my goal to make this emulator as accurate as possible but any help would be appreciated!

## Prerequisites

The following libraries should be installed on your system:
* SDL2

Cmake and Make is needed to be able to build the emulator

## Build Instructions

Clone the CHIP-8 respository

~~~
git clone https://github.com/robbelbobel/CHIP-8-Emu
~~~

Navigate into the CHIP-8-Emu folder

~~~
cd CHIP-8-Emu/
~~~

Create a 'build' folder and enter it

~~~
mkdir build
cd build/
~~~

Intialize build folder using cmake and build using make

~~~
cmake ..
make
~~~

## Usage

The emulator should be executed using the terminal with the following syntax:




TYPE(NOOR IS COOL)

~~~
./CHIP-8 [OPTIONAL:ROMPATH]
~~~

The ROM can also be loaded by dragging and dropping the game in the window

## TO DO

* Implement Audio
* Code Cleanup and optimization (Implement "switch-statement" instead of "if else")
