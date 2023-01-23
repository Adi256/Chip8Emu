#pragma once

#include <conio.h>
#include <bitset>
#include"Graphics.h"

class ConsoleGraphics : public Graphics
{
	bool clearConsoleOnDrawCall = false;
public:
	ConsoleGraphics(bool clearConsoleOnDraw)
	{
		clearConsoleOnDrawCall = clearConsoleOnDraw;
	}
	void drawScreen(bool clearBeforeDraw);
};