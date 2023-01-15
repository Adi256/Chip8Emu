#include "Chip8.h"

const char chip8FontSet[80] = {0,0,0,0,0,0,0,0,0,0,
							   0,0,0,0,0,0,0,0,0,0,
							   0,0,0,0,0,0,0,0,0,0,
							   0,0,0,0,0,0,0,0,0,0,
							   0,0,0,0,0,0,0,0,0,0,
							   0,0,0,0,0,0,0,0,0,0,
							   0,0,0,0,0,0,0,0,0,0,
							   0,0,0,0,0,0,0,0,0,0};

void Chip8::clearMemory()
{
	for (int i = 0x200; i < MEMORY_SIZE; i++)
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

void Chip8::loadFontSet()
{
	for (int i = 0; i < 80; i++)
	{
		memory[i] = chip8FontSet[i];
	}
}

void Chip8::execute0x8Opcodes()
{
	switch (opcode & 0x000F)
	{
	case 0x0000:
		V[opcode & 0x0F00] = V[opcode & 0x00F0];
		pc += 2;
		break;

	case 0x0001:
		V[opcode & 0x0F00] |= V[opcode & 0x00F0];
		pc += 2;
		break;

	case 0x0002:
		V[opcode & 0x0F00] &= V[opcode & 0x00F0];
		pc += 2;
		break;

	case 0x0003:
		V[opcode & 0x0F00] ^= V[opcode & 0x00F0];
		pc += 2;
		break;
	}
}

void Chip8::executeOpcode()
{
	switch (opcode & 0xF000)
	{
	case 0x0000:
		switch (opcode & 0x000F)
		{
		case 0x0000:
			graphicsController->clearScreen();
			pc += 2;
			break;
		//Coming back from a subroutine
		case 0x000E:
			pc = stack[--sp];
			pc += 2;
			break;
		default:
			std::cout << "Unknown opcode: " << opcode << std::endl;
		}

	case 0xA000:
		I = opcode & 0x0FFF;
		pc += 2;
		break;

	//Subroutine call
	case 0x2000:
		stack[sp++] = pc;
		pc = opcode & 0x0FFF;
		break;
	
	//Adds the value of VY to VX
	case 0x8000:
		execute0x8Opcodes();
		break;

	default:
		std::cout << "Unknown opcode: " << opcode << std::endl;
	}
}
