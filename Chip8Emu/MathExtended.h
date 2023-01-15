#include <cmath>

class MathExtended
{
public:
	static int randomNumber(int min, int max)
	{
		return rand() % (max - min) + min;
	}
};