#include "ConsoleGraphics.h"

void ConsoleGraphics::drawScreen(bool clearBeforeDraw)
{
	if(clearBeforeDraw)
		system("cls");
	for (unsigned char row = 0; row < SCREEN_H; row++)
	{
		std::cout << std::bitset<sizeof(gfx[row]) * CHAR_BIT>(gfx[row]) << std::endl;
	}
	std::cout << std::endl;
}
