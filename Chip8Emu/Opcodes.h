#define JP 0x1000 //Jump to nnn (0x1nnn)
#define CALL 0x2000 //Subroutine call (0x2nnn)
#define SE 0x3000 //Skip next instruction if Vx == kk (0x3xkk)
#define SNE 0x4000 //Skip next instruction if Vx != kk
#define VSE 0x5000 //Skip next instruction if Vx == Vy
#define LD 0x6000 //The interpreter puts the value kk into register Vx
#define ADD 0x7000 	//Adds the value kk to the value of register Vx, then stores the result in Vx

//0x8 Opcodes

#define LD_8 0x0000 //Stores the value of register Vy in register Vx.(0x8xy0)
#define OR_8 0x0001 //Set Vx = Vx OR Vy. (0x8xy1)
#define AND_8 0x0002 //Set Vx = Vx AND Vy.(0x8xy2)
#define XOR_8 0x0003 //Set Vx = Vx XOR Vy. (0x8xy3)
#define ADD_8 0x0004 //Set Vx = Vx + Vy, set VF = carry. (0x8xy4)
#define SUB_8 0x0005 //Set Vx = Vx - Vy, set VF = NOT borrow. (0x8xy5)
#define SHR_8 0x0006 //Set Vx = Vx SHR 1. (0x8xy6)
#define SUBN_8 0x0007 //Set Vx = Vy - Vx, set VF = NOT borrow. (0x8xy7)
#define SHL_8 0x000E // Set Vx = Vx SHL 1. (0x8xyE)
