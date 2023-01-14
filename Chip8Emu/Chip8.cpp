#include "Chip8.h"

void Chip8::clearMemory()
{
	for (int i = 0; i < MEMORY_SIZE; i++)
	{
		memory[i] = 0;
	}
}

void Chip8::clearRegisters()
{
	for (int i = 0; i < REGISTER_AMOUNT; i++)
	{
		V[i] = 0;
	}
}

void Chip8::clearStack()
{
	for (int i = 0; i < STACK_SIZE; i++)
	{
		stack[i] = 0;
	}
}

void Chip8::clearKeysState()
{
	for (int i = 0; i < PHYSICAL_KEY_AMOUNT; i++)
	{
		key[i] = 0;
	}
}