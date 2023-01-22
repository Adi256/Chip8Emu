#include <iostream>
#include "Chip8.h"
#include "ConsoleGraphics.h"
#include "WindowsKeyboard.h"
#include "ConsoleSound.h"

int main(int argc, char** argv)
{
    ConsoleGraphics graphics(false);
    WindowsKeyboard keyboard;
    ConsoleSound sound;

    //keyboard.userMappingMenu();

    Chip8 chip(&graphics, &sound);

    if (chip.loadProgramIntoMemory("Programs/Delay Timer Test [Matthew Mikolay, 2010].ch8") == false)
    {
        std::cout << "Failed to load the program into Chip-8 memory!" << std::endl;
        return -1;
    }
    
    chip.debugRun(1000, true);

    return 0;
}