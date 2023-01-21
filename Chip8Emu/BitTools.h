#pragma once

#include <math.h>
#include <climits>
#include <bitset>
#include <iostream>

template<typename T>
class BitTools
{
public:
	//Wraps bits in a variable. Fex 11010 >> 3 -> 01011
	//Positive shiftBy shifts to the right, whereas negative to the left.
	static void bitWrap(T &toWrap, char shiftBy)
	{
		if (shiftBy > 0)
		{
			shiftBy %= sizeof(toWrap) * CHAR_BIT; //We wrap the number of bits shifted to the total amount of bits in a variable.
			T mask = pow(2, shiftBy) - 1; //We are creating a mask that will envelope the bits that are supposed to be shifted to the right.
			T ending = toWrap & mask; //Here we are collecting those bits.
			toWrap >>= shiftBy; //Here we are performing the shift itself. The bits from above are lost for now.
			toWrap |= (ending << (sizeof(toWrap) * CHAR_BIT - shiftBy)); // Here we are "copy pasting" those bits to the beginning of our variable.
		}
		else if (shiftBy < 0)
		{
			shiftBy = -shiftBy;
			shiftBy %= sizeof(toWrap) * CHAR_BIT;
			T mask = pow(2, shiftBy) - 1;
			mask <<= (sizeof(toWrap) * CHAR_BIT - shiftBy); //We are shifting the mask to the beginning of the variable.
			T start = toWrap & mask;
			toWrap <<= shiftBy;
			toWrap |= (start >> (sizeof(toWrap) * CHAR_BIT - shiftBy));
		}
	}
};