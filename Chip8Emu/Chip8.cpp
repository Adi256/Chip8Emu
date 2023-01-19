#include "Chip8.h"

//Opcodes and documentation comes from http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

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

//Executes opcodes that start with hex 0x8
void Chip8::execute0x8Opcodes()
{
	switch (opcode & 0x000F)
	{
	case 0x0000:
		V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
		pc += 2;
		break;

	//Bitwise operations
	case 0x0001:
		V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
		pc += 2;
		break;

	case 0x0002:
		V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
		pc += 2;
		break;

	case 0x0003:
		V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
		pc += 2;
		break;

	// VX += VY with carry
	case 0x0004:
		//Reseting the overflow register
		V[0xF] = 0;
		//Setting the overflow register if overflow happens
		if (0xFF - V[(opcode & 0x0F00) >> 8] < V[(opcode & 0x00F0) >> 4])
			V[0xF] = 1;

		V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
		pc += 2;
		break;

	// VX -= VY with borrow
	case 0x0005:
		//Checks for borrow
		if (0xFF - V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
			V[0xF] = 0;
		else
			V[0xF] = 1;

		V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
		pc += 2;
		break;

	case 0x0006:
		//Store the least significant bit in the carry register
		V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;

		//Shift the register content one to the right(binary)
		V[(opcode & 0x0F00) >> 8] >>= 1;
		pc += 2;
		break;
	
	// VX = VY - VX with borrow
	case 0x0007:
		//Checks for borrow
		if (0xFF - V[(opcode & 0x0F00) >> 8] < V[(opcode & 0x00F0)])
			V[0xF] = 0;
		else
			V[0xF] = 1;

		V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
		pc += 2;
		break;

	case 0x000E:
		//Store the most significant bit in the carry register
		V[0xF] = V[(opcode & 0x0F00) >> 8] & 0xFF;

		//Shift the register content one to the left(binary)
		V[(opcode & 0x0F00) >> 8] <<= 1;
		pc += 2;
		break;
	}
}

//Executes opcodes that start with hex 0xE
void Chip8::execute0xEOpcodes()
{
	switch (opcode & 0x000F)
	{
	//Skip the next instruction if the key key[VX] is pressed
	case 0x000E:
		if (key[(opcode & 0x0F00) >> 8])
			pc += 4;
		else
			pc += 2;
		break;

	//Skip the next instruction if the key key[VX] is not pressed
	case 0x0001:
		if (key[(opcode & 0x0F00) >> 8])
			pc += 2;
		else
			pc += 4;
		break;

	}
}

//Executes opcodes that start with hex 0xF
void Chip8::execute0xFOpcodes()
{
	//Variable either modified or read from for every 0xF opcode
	unsigned char &Vx = V[(opcode & 0x0F00) >> 8];
	switch (opcode & 0x00FF)
	{
	case 0x0007:
		Vx = delayTimer;
		break;
	//Freezes the program until user interacts with it by clicking a key.
	case 0x000A:
		Vx = keyboardController->waitForKeyPress();
		break;
	case 0x0015:
		delayTimer = Vx;
		break;
	case 0x0018:
		soundTimer = Vx;
		break;
	case 0x001E:
		I += Vx;
		break;
	case 0x0033:
		memory[I] = Vx / 100;
		memory[I + 1] = (Vx - memory[I] * 100) / 10;
		memory[I + 2] = Vx - (memory[I] * 100 + memory[I + 1] * 10);
		break;
	//Read registers V0 through Vx from memory starting at location I
	case 0x0055:
		for(int x = 0; x <= ((opcode & 0x0F00) >> 8); x++)
		{
			V[x] = memory[I + x];
		}
		break;
	}

	pc += 2;
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

	//Jump to nnn (0x1nnn)
	case 0x1000:
		pc = opcode & 0x0FFF;
		break;

	//Subroutine call
	case 0x2000:
		stack[sp++] = pc;
		pc = opcode & 0x0FFF;
		break;

	//Skip next instuction if Vx == kk (0x3xkk)
	case 0x3000:
		if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
			pc += 4;
		else
			pc += 2;
		break;

	//Skip next instruction if Vx != kk
	case 0x4000:
		if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
			pc += 4;
		else
			pc += 2;
		break;
		
	//Skip next instruction if Vx == Vy
	case 0x5000:
		if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
			pc += 4;
		else
			pc += 2;
		break;

	//The interpreter puts the value kk into register Vx
	case 0x6000:
		V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
		break;

	//Adds the value kk to the value of register Vx, then stores the result in Vx
	case 0x7000:
		V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
		break;

	//Adds the value of VY to VX
	case 0x8000:
		execute0x8Opcodes();
		break;

	//Skip the next instruction if VX != VY
	case 0x9000:
		if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
			pc += 2;
		else
			pc += 4;
		break;

	case 0xA000:
		I = opcode & 0x0FFF;
		pc += 2;
		break;

	case 0xB000:
		pc = V[0] + opcode & 0x0FFF;
		break;

	case 0xC000:
		V[(opcode & 0x0F00) >> 8] = MathExtended::randomNumber(0, 256) & (opcode & 0x00FF);
		pc += 2;
		break;

	case 0xE000:
		execute0xEOpcodes();
		break;

	case 0xF000:
		execute0xFOpcodes();

	default:
		std::cout << "Unknown opcode: " << opcode << std::endl;
	}
}
