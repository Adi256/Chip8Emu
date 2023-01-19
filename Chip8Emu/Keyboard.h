class Keyboard
{
public:
	//Freezes a program until provided with a keycode. Returns the key pressed.
	virtual char waitForKeyPress() = 0;
};