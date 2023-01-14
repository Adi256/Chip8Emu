#pragma once

#include"Graphics.h"

#define MEMORY_SIZE 4096
//Chip has 15 8-bit general purpose registers, plus one extra for carry flag
#define REGISTER_AMOUNT 16
#define STACK_SIZE 16
#define PHYSICAL_KEY_AMOUNT 16

class Chip8
{
	//Used to store the current opcode
	unsigned short opcode;

	//Simulates a memory chip
	unsigned char memory[MEMORY_SIZE];
	void clearMemory();

	/*System memory map:
	* 0x000-0x1FF Font set
	* 0x200-0xFFF Program ROM and work RAM
	*/

	//Chip registers.
	unsigned char V[REGISTER_AMOUNT];
	//Index register
	unsigned short I;
	void clearRegisters();

	//Program counter
	unsigned short pc;

	/*Reference to a class responsible for displaying contents onto a screen.
	* Expects a child class that implements all of the virtual functions.
	*/
	Graphics* graphicsController = nullptr;

	//Timer register that counts to 0 when set above that value.
	unsigned char delayTimer;
	//Timer register that counts to 0 when set above that value, then beeps.
	unsigned char soundTimer;

	//Used to remember a previous position of an index register before it jumps into a subroutine.
	unsigned short stack[STACK_SIZE];
	void clearStack();
	//A stack pointer, points at a current position in a stack.
	unsigned short sp;

	//Stores a current state of a key
	bool key[PHYSICAL_KEY_AMOUNT];
	void clearKeysState();

	void resetChip()
	{
		opcode = 0;

		clearMemory();

		clearRegisters();
		I = 0; // Reset index register

		pc = 0x200; // Program starts at 0x200, memory before that is used for storing internal information such as font data.

		//We are reseting the timers.
		delayTimer = 0;
		soundTimer = 0;
		
		clearStack();
		sp = 0; //Reseting the stack pointer.
	}

public:

	Chip8(Graphics* graphics)
	{
		graphicsController = graphics;
	}
};