#include "WindowsKeyboard.h"

void WindowsKeyboard::userMappingMenu()
{
	for (int key = 0; key < PHYSICAL_KEY_AMOUNT; key++)
	{
		std::cout << "Click a key on your keyboard that will be mapping to " << key << " in the chip-8 emulator." << std::endl;
		characterMapping.insert({ _getch(), key });
	}
}

bool WindowsKeyboard::loadMappingFromFile(const char* fileName)
{
	characterMapping.clear();
	std::ifstream keyMapping(fileName, std::ios::binary);

	if (keyMapping.is_open() == false)
	{
		std::cout << "Failed to load keymappings, failed to load the file!" << std::endl;
		return false;
	}

	for (unsigned char key = 0; key < PHYSICAL_KEY_AMOUNT; key++)
	{
		unsigned char keyCode = 0;
		keyMapping >> keyCode;
		if (keyMapping.eof())
		{
			std::cout << "Failed to load keymappings, file lacks keycodes!" << std::endl;
			return false;
		}

		characterMapping.emplace(std::pair<unsigned char, unsigned char>{ keyCode, key });
	}

	return true;
}
