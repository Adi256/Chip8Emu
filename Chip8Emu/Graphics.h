#pragma once

#define SCREEN_W 64
#define SCREEN_H 32

class Graphics
{
	//Holds information about the current contents of a screen
	//Note that the display is black and white only
	unsigned char gfx[SCREEN_W * SCREEN_H];
public:
	//Turns the pixel either on or off. Warning, the function can take pixels values that are out of range!
	void setPixel(char x, char y, unsigned char setTo)
	{
		gfx[y * SCREEN_W + x] = setTo;
	}
	//Clears the gfx data(sets all to 0)
	void clearGFX();

	virtual void drawScreen() = 0;

	Graphics();
};