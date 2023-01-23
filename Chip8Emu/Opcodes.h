#define JP 0x1000 //Jump to nnn (0x1nnn)
#define CALL 0x2000 //Subroutine call (0x2nnn)
#define SE 0x3000 //Skip next instruction if Vx == kk (0x3xkk)
#define SNE 0x4000 //Skip next instruction if Vx != kk
#define VSE 0x5000 //Skip next instruction if Vx == Vy
#define LDVX 0x6000 //The interpreter puts the value kk into register Vx
#define ADD 0x7000 	//Adds the value kk to the value of register Vx, then stores the result in Vx
#define SNEVXVY 0x9000 //Skip next instruction if Vx != Vy.
#define LDI 0xA000 //Set I = nnn
#define JPV0 0xB000 //Jump to location nnn + V0
#define RND 0xC000 //Set Vx = random byte AND kk.
#define DRW 0xD000 //Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

//0x0 opcodes
#define CLS_0 0x00E0 //Clear the display.
#define RET_0 0x00EE //Return from a subroutine

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

//0xE opcodes
#define SKP_E 0x009E //Skip next instruction if key with the value of Vx is pressed.
#define SKNP_E 0x00A1 //Skip next instruction if key with the value of Vx is not pressed.

//0xF opcodes
#define LDVXDT_F 0x0007 //Set Vx = delay timer value.
#define LDVXK_F 0x000A //Wait for a key press, store the value of the key in Vx.
#define LDDTVX_F 0x0015 //Set delay timer = Vx.
#define LDSTVS_F 0x0018 //Set sound timer = Vx.
#define ADD_F 0x001E //Set I = I + Vx.
#define LDFVX_F 0x0029 //Set I = location of sprite for digit Vx.
#define LDBVX_F 0x0033 //Store BCD representation of Vx in memory locations I, I+1, and I+2.
#define LDIVX_F 0x0055 //Store registers V0 through Vx in memory starting at location I.
#define LDVXI_F 0x0065 //Read registers V0 through Vx from memory starting at location I.