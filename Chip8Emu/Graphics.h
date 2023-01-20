#pragma once

#include"BitTools.h"

#define SCREEN_W 64 //TODO: Implement variable screen-size using <bitset>
#define SCREEN_H 32

class Graphics
{
	//Holds information about the current contents of a screen
	//Note that the display is black and white only
	unsigned long long gfx[SCREEN_H];
public:
	//Clears the gfx data(sets all to 0)
	void clearGFX();

	//Xors a row of pixels. That is it performs a xor operation between 8bit excrept from the gfx array and 8bit row variable
	//Returns true if collision was detected(1->0)
	bool xorRow(unsigned char x, unsigned char y, unsigned char row)
	{
		unsigned long long longRow = row;
		BitTools<unsigned long long>::bitWrap(longRow, x); //Here we are shifting start of the row by its expected position. With this action, start of the row is where its x-coordinate is meant to be. We are also wrapping the row just as it is provided in the documentation.
		y %= SCREEN_H; //More wrapping.

		unsigned long long tempRow = gfx[y];
		gfx[y] ^= longRow; //Xoring just as described in the documentation
		/*
		* In this line we are anding the two rows. Let's say that the only change was that one of the bits turned 0->1.
		* In that case, change = gfx[y]. Now, let's say that one of the bits 1->0, in this case change < gfx[y].
		* Now let's say that one bit 0->1(change = gfx[y]), but also after that one bit 1->0(change < gfx[y]).
		* This means that if any of the bits has turned into 0, change < gfx[y], and if it didnt, change = gfx[y].
		*/
		unsigned long long change = tempRow & gfx[y];
		if (gfx[y] - change > 0)
			return true;
		else
			return false;
	}

	virtual void drawScreen() = 0;

	Graphics();
};