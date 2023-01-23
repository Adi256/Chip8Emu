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

    Chip8 chip(&graphics, &sound, &keyboard);

    if (keyboard.loadMappingFromFile("Config/keymapping.txt") == false)
        return -1;

    if (chip.loadProgramIntoMemory("Programs/BMP Viewer - Hello (C8 example) [Hap, 2005].ch8") == false)
    {
        std::cout << "Failed to load the program into Chip-8 memory!" << std::endl;
        return -1;
    }
    
    chip.run(-1, true);

    return 0;
}