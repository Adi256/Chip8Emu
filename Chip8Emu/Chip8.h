#pragma once

#include <iostream>

#include <thread>
#include <chrono>

#include <fstream>
#include <sstream>

#include"Graphics.h"
#include"Sound.h"
#include"Keyboard.h"
#include"MathExtended.h"

#define MEMORY_SIZE 4096
//Chip has 15 8-bit general purpose registers, plus one extra for carry flag
#define REGISTER_AMOUNT 16
#define STACK_SIZE 16

//Frequency in Hz, source: https://github.com/AfBu/haxe-CHIP-8-emulator/wiki/(Super)CHIP-8-Secrets#speed-of-emulation
#define CHIP_FREQUENCY 500
#define TIMER_FREQUENCY 60

using namespace std::chrono;

class UnknownOpcodeException : std::exception
{
public:
	UnknownOpcodeException(int opcode) : unknownOpcode(opcode) {}

	int unknownOpcode;
	const char* what()
	{
		return "Tried to execute an unknown opcode!";
	}
};

class Chip8
{
	//Used to store the current opcode
	unsigned short opcode;

	//Simulates a memory chip
	unsigned char memory[MEMORY_SIZE];
	void clearMemory();

	/*System memory map:
	* 0x000-0x200 Font set
	* 0x200-0xFFF Program ROM and work RAM
	*/

	//Chip registers.
	unsigned char V[REGISTER_AMOUNT];
	//Index register
	unsigned short I;
	void clearRegisters();

	//Program counter
	unsigned short pc;

	/*References to classes that handle IO with different hardware.
	*/
	Graphics* graphicsController = nullptr;
	Sound* soundController = nullptr;
	Keyboard* keyboardController = nullptr;

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
	unsigned char key[PHYSICAL_KEY_AMOUNT];
	void clearKeysState();

	void resetChip()
	{
		std::cout << "Reseting the chip..." << std::endl;
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

		std::cout << "Chip reset was successful." << std::endl;
	}
	
	//Loads a font set into a memory, making it act as if its burned into it by a manufacturer.
	void loadFontSet();

	void fetchOpcode()
	{
		//Each opcode consists of two bytes.
		opcode = memory[pc] << 8 | memory[pc + 1];
	}

	void execute0x8Opcodes();
	void execute0xEOpcodes();
	void execute0xFOpcodes();
	void executeOpcode();

	void updateTimers()
	{
		if (delayTimer > 0)
			delayTimer--;
		if (soundTimer > 0)
		{
			soundController->beep();
			soundTimer--;
		}
	}

	//Returns false if the opcode failed to execute
	bool emulateCycle(bool shouldUpdateTimers, bool renderScreen = true)
	{	
		fetchOpcode();
		pc += 2;
		try
		{
			executeOpcode();
		}
		catch (UnknownOpcodeException uoe)
		{
			std::cout << uoe.what() << std::endl;
			std::cout << uoe.unknownOpcode << std::endl;
			return false;
		}

		if(shouldUpdateTimers)
			updateTimers();

		if(renderScreen)
			graphicsController->drawScreen();

		return true;
	}

public:

	Chip8(Graphics* graphics, Sound* sound)
	{
		graphicsController = graphics;
		resetChip();
		loadFontSet();
	}
	
	//Runs the chip for a given number of cycles 
	void run(int cycles, bool renderScreen);
	//Only prints out memory addresses until pc
	void debugRun(int cycles, bool renderScreen);

	void debugLoadOpcodesMenu(unsigned char numberOfOpcodes);
	bool loadProgramIntoMemory(const char* programName);
};