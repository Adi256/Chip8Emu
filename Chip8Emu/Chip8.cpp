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

	//Bitwise operations
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

	// VX += VY with carry
	case 0x0004:
		//Reseting the overflow register
		V[0xF] = 0;
		//Setting the overflow register if overflow happens
		if (0xFF - V[opcode & 0x0F00] < V[opcode & 0x00F0])
			V[0xF] = 1;

		V[opcode & 0x0F00] += V[opcode & 0x00F0];
		pc += 2;
		break;

	// VX -= VY with borrow
	case 0x0005:
		//Checks for borrow
		if (0xFF - V[opcode & 0x0F00] > V[opcode & 0x00F0])
			V[0xF] = 0;
		else
			V[0xF] = 1;

		V[opcode & 0x0F00] -= V[opcode & 0x00F0];
		pc += 2;
		break;

	case 0x0006:
		//Store the least significant bit in the carry register
		V[0xF] = V[opcode & 0x0F00] & 0x1;

		//Shift the register content one to the right(binary)
		V[opcode & 0x0F00] >>= 1;
		pc += 2;
		break;
	
	// VX = VY - VX with borrow
	case 0x0007:
		//Checks for borrow
		if (0xFF - V[opcode & 0x0F00] < V[opcode & 0x00F0])
			V[0xF] = 0;
		else
			V[0xF] = 1;

		V[opcode & 0x0F00] = V[opcode & 0x00F0] - V[opcode & 0x0F00];
		pc += 2;
		break;

	case 0x000E:
		//Store the most significant bit in the carry register
		V[0xF] = V[opcode & 0x0F00] & 0xFF;

		//Shift the register content one to the left(binary)
		V[opcode & 0x0F00] <<= 1;
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
