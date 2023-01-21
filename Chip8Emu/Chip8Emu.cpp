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
    
    chip.debugLoadOpcodesMenu(4);
    chip.run(4, true);

    return 0;
}