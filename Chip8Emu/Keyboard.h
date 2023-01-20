class Keyboard
{
public:
	//Freezes a program until provided with a keycode. Returns the key pressed.
	virtual unsigned char waitForKeyPress() = 0;
};