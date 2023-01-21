#pragma once
#include<conio.h>
#include<map>
#include<stdexcept>
#include<iostream>
#include"Keyboard.h"

class WindowsKeyboard : public Keyboard
{
	std::map<unsigned char, unsigned char> characterMapping; //Mapping from keyboard input into Chip-8 characters.
public:

	void addMapping(unsigned char keyboardCharacter, unsigned char chip8Mapping)
	{
		characterMapping.insert({ keyboardCharacter, chip8Mapping });
	}
	unsigned char waitForKeyPress()
	{
		unsigned char keyboard = _getch();
		unsigned char character = 0;
		try
		{
			character = characterMapping.at(keyboard);
		}
		catch (const std::out_of_range& oor)
		{
			std::cout << "Unmapped key pressed on a keyboard! Returning 0." << std::endl;
		}

		return character;
	}

	//Allows user to map custom keyboard keys for chip-8
	void userMappingMenu();
};