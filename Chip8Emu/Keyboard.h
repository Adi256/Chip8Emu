#pragma once
#define PHYSICAL_KEY_AMOUNT 16

class Keyboard
{
public:
	//Freezes a program until provided with a keycode. Returns the key pressed.
	virtual unsigned char waitForKeyPress() = 0;
};