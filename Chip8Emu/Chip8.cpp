#include "Chip8.h"
#include "Opcodes.h"

//Opcodes and documentation comes from http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

//Fontset from https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
unsigned char chip8_fontset[80] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void Chip8::clearMemory()
{
	std::cout << "Clearing the memory..." << std::endl;
	for (int i = 0x200; i < MEMORY_SIZE; i++)
	{
		memory[i] = 0;
	}
	std::cout << "Memory cleared!" << std::endl;
}

void Chip8::clearRegisters()
{
	std::cout << "Clearing the registers..." << std::endl;
	for (unsigned char i = 0; i < REGISTER_AMOUNT; i++)
	{
		V[i] = 0;
	}
	std::cout << "Registers cleared!" << std::endl;
}

void Chip8::clearStack()
{
	std::cout << "Clearing the stack..." << std::endl;
	for (unsigned char i = 0; i < STACK_SIZE; i++)
	{
		stack[i] = 0;
	}
	std::cout << "Stack cleared!" << std::endl;
}

void Chip8::clearKeysState()
{
	std::cout << "Clearing the key states..." << std::endl;
	for (unsigned char i = 0; i < PHYSICAL_KEY_AMOUNT; i++)
	{
		key[i] = 0;
	}
	std::cout << "Key states cleared!" << std::endl;
}

void Chip8::loadFontSet()
{
	std::cout << "Loading the font set..." << std::endl;
	for (unsigned char i = 0; i < 80; i++)
	{
		memory[i] = chip8_fontset[i];
	}
	std::cout << "Font set loaded!" << std::endl;
}

//Executes opcodes that start with hex 0x8
void Chip8::execute0x8Opcodes()
{
	switch (opcode & 0x000F)
	{
	case LD_8:
		V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
		break;

	case OR_8:
		V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
		break;

	case AND_8:
		V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
		break;

	case XOR_8:
		V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
		break;

	case ADD_8:
		//Reseting the overflow register
		V[0xF] = 0;
		//Setting the overflow register if overflow happens
		if (0xFF - V[(opcode & 0x0F00) >> 8] < V[(opcode & 0x00F0) >> 4])
			V[0xF] = 1;
		V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
		break;

	case SUB_8:
		//Checks for borrow
		if (0xFF - V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
			V[0xF] = 0;
		else
			V[0xF] = 1;
		V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
		
		break;

	case SHR_8:
		//Store the least significant bit in the carry register
		V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;

		//Shift the register content one to the right(binary)
		V[(opcode & 0x0F00) >> 8] >>= 1;
		break;

	case SUBN_8:
		//Checks for borrow
		if (0xFF - V[(opcode & 0x0F00) >> 8] < V[(opcode & 0x00F0)])
			V[0xF] = 0;
		else
			V[0xF] = 1;

		V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
		
		break;

	case SHL_8:
		//Store the most significant bit in the carry register
		V[0xF] = V[(opcode & 0x0F00) >> 8] & 0xFF;

		//Shift the register content one to the left(binary)
		V[(opcode & 0x0F00) >> 8] <<= 1;
		break;

	default:
		throw UnknownOpcodeException(opcode);
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
			pc += 2;
		break;

	//Skip the next instruction if the key key[VX] is not pressed
	case 0x0001:
		if (!key[(opcode & 0x0F00) >> 8])
			pc += 2;
		break;
	default:
		throw UnknownOpcodeException(opcode);
	}
}

//Executes opcodes that start with hex 0xF
void Chip8::execute0xFOpcodes()
{
	//Register ID
	unsigned char rID = (opcode & 0x0F00) >> 8;
	switch (opcode & 0x00FF)
	{
	case 0x0007:
		V[rID] = delayTimer;
		break;
	//Freezes the program until user interacts with it by clicking a key.
	case 0x000A:
		V[rID] = keyboardController->waitForKeyPress();
		break;
	case 0x0015:
		delayTimer = V[rID];
		break;
	case 0x0018:
		soundTimer = V[rID];
		break;
	case 0x001E:
		I += V[rID];
		break;
	//The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.
	case 0x0029:
		I = V[rID] * 5;
		break;
	case 0x0033:
		memory[I] = V[rID] / 100;
		memory[I + 1] = (V[rID] - memory[I] * 100) / 10;
		memory[I + 2] = V[rID] - (memory[I] * 100 + memory[I + 1] * 10);
		break;
		//Read registers V0 through Vx from memory starting at location I
	case 0x0055:
		for (int x = 0; x <= rID; x++)
		{
			memory[I + x] = V[x];
		}
		break;
	//Read registers V0 through Vx from memory starting at location I
	case 0x0065:
		for(int x = 0; x <= rID; x++)
		{
			V[x] = memory[I + x];
		}
		break;
	default:
		throw UnknownOpcodeException(opcode);
	}

	
}

void Chip8::executeOpcode()
{
	switch (opcode & 0xF000)
	{
	case 0x0000:
		switch (opcode & 0x00FF)
		{
		case 0x00E0:
			graphicsController->clearGFX();
			break;
		//Coming back from a subroutine
		case 0x00EE:
			pc = stack[--sp];
			break;
		default:
			throw UnknownOpcodeException(opcode);
		}
		break;

	case JP:
		pc = opcode & 0x0FFF;
		break;

	case CALL:
		stack[sp++] = pc;
		pc = opcode & 0x0FFF;
		break;

	case SE:
		if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
			pc += 2;
		break;

	case SNE:
		if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
			pc += 2;
		break;
		
	case VSE:
		if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
			pc += 2;
		break;

	case LD:
		V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
		break;

	case ADD:
		V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
		break;

	//Adds the value of VY to VX
	case 0x8000:
		execute0x8Opcodes();
		break;

	//Skip the next instruction if VX != VY
	case 0x9000:
		if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
			pc += 2;
		break;

	case 0xA000:
		I = opcode & 0x0FFF;
		break;

	case 0xB000:
		pc = V[0] + opcode & 0x0FFF;
		break;

	case 0xC000:
		V[(opcode & 0x0F00) >> 8] = MathExtended::randomNumber(0, 256) & (opcode & 0x00FF);
		break;

	case 0xD000:
	{
		unsigned char rows = opcode & 0x000F;
		unsigned char collisions = 0;
		for (unsigned char row = 0; row < rows; row++)
		{
			collisions += graphicsController->xorRow(V[(opcode & 0x0F00) >> 8],
				V[(opcode & 0x00F0) >> 4] + row,
				memory[I + row]);
		}
		if (collisions > 0)
		{
			V[0xF] = 1;
		}
		break;
	}

	case 0xE000:
		execute0xEOpcodes();
		break;

	case 0xF000:
		execute0xFOpcodes();
		break;

	default:
		throw UnknownOpcodeException(opcode);
	}
}

void Chip8::run(int cycles, bool renderScreen)
{
	for (int cycle = 0; cycle < cycles; cycle++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / TIMER_FREQUENCY));
		if (emulateCycle(true, renderScreen) == false)
			return;
	}
}

void Chip8::debugRun(int cycles, bool renderScreen)
{
	std::cout << "Font memory: " << std::endl;
	for (int mem = 0; mem < 0x200; mem++)
	{
		std::cout << " " << memory[mem] << " ";
	}
	std::cout << std::endl;

	std::cout << "Program memory: " << std::endl;
	for (int cycle = 0; cycle < cycles; cycle++)
	{
		if (emulateCycle(true, renderScreen) == false)
			return;
		std::cout << "PC: " << pc << " memory: " << opcode << std::endl;
	}
}

void Chip8::debugLoadOpcodesMenu(unsigned char numberOfOpcodes)
{
	for (unsigned char _opcode = 0; _opcode < numberOfOpcodes; _opcode++)
	{
		std::cout << "Opcode: " << std::endl;
		int code = 0;
		std::cin >> code;
		memory[0x200 + _opcode * 2] = code >> 8;
		memory[0x200 + _opcode * 2 + 1] = code;
	}
}

bool Chip8::loadProgramIntoMemory(const char* programName)
{
	std::ifstream program(programName, std::ios::binary);
	int memoryAddr = 0x200; //Currently modified memory address.

	if (program.is_open() == false)
		return false;

	std::stringstream buffer;
	buffer << program.rdbuf();

	for (int ch = 0; ch < buffer.str().length(); ch++)
	{
		memory[0x200 + ch] = buffer.str()[ch];
	}

	return true;
}
