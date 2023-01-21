#include "Graphics.h"

void Graphics::clearGFX()
{
	for (int i = 0; i < SCREEN_H; i++)
	{
		gfx[i] = 0;
	}
}

Graphics::Graphics()
{
	clearGFX();
}
