#pragma once
#include<iostream>

#include "Sound.h"

class ConsoleSound : public Sound
{
	void beep()
	{
		std::cout << "Beep!" << std::endl;
	}
};