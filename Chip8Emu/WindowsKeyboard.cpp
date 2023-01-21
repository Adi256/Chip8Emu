#include "WindowsKeyboard.h"

void WindowsKeyboard::userMappingMenu()
{
	for (int key = 0; key < PHYSICAL_KEY_AMOUNT; key++)
	{
		std::cout << "Click a key on your keyboard that will be mapping to " << key << " in the chip-8 emulator." << std::endl;
		characterMapping.insert({ _getch(), key });
	}
}
