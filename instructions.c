#include "include/chip8.h"
#include <stdint.h>

//  0nnn - SYS addr -  Unimplemented, useless.

/*
00EE - RET
Return from a subroutine.

The interpreter sets the program counter to the address at the top of the stack, 
then subtracts 1 from the stack pointer.
*/
void RET()
{
    p_cpu->pc = p_cpu->stack[p_cpu->sp];
    p_cpu->sp--;
}

/*
00E0 - CLS
Clear the display.
*/
void CLS()
{
    memset(p_cpu->display, 0, sizeof(p_cpu->display));
}

/*
1nnn - JP addr
Jump to location nnn.

The interpreter sets the program counter to nnn.
*/
void JP(int16_t addr)
{
    p_cpu->pc = addr;
}

/*
2nnn - CALL addr
Call subroutine at nnn.

The interpreter increments the stack pointer, 
then puts the current PC on the top of the stack. 
The PC is then set to nnn.
*/
void CALL(int16_t addr)
{
    p_cpu->sp++;
    p_cpu->stack[p_cpu->sp] = p_cpu->pc;
    p_cpu->pc = addr;
}

/*
3xkk - SE Vx, byte
Skip next instruction if Vx = kk.

The interpreter compares register Vx to kk, and if they are equal, 
increments the program counter by 2.
*/
void bSE(int8_t Vx, int8_t kk)
{
    if(p_cpu->r[Vx] == kk) p_cpu->pc += 2;
}

/*
4xkk - SNE Vx, byte
Skip next instruction if Vx != kk.

The interpreter compares register Vx to kk, and if they are not equal, 
increments the program counter by 2.
*/
void bSNE(int8_t Vx, int8_t kk)
{
    if(p_cpu->r[Vx] != kk) p_cpu->pc += 2;
}

/*
5xy0 - SE Vx, Vy
Skip next instruction if Vx = Vy.

The interpreter compares register Vx to register Vy, and if they are equal, 
increments the program counter by 2.
*/
void SE(int8_t Vx, int8_t Vy) {
    if(p_cpu->r[Vx] == p_cpu->r[Vy]) p_cpu->pc += 2;
}

/*
6xkk - LD Vx, byte
Set Vx = kk.

The interpreter puts the value kk into register Vx.
*/
void bLD(int8_t Vx, int8_t kk)
{
    p_cpu->r[Vx] = kk;
}

/*
7xkk - ADD Vx, byte
Set Vx = Vx + kk.

Adds the value kk to the value of register Vx, then stores the result in Vx. 
*/
void bADD(int8_t Vx, int8_t kk)
{
    p_cpu->r[Vx] = p_cpu->r[Vx] + kk;
}

/*
8xy1 - OR Vx, Vy
Set Vx = Vx OR Vy.

Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0.
*/
void OR(int8_t Vx, int8_t Vy) 
{
    p_cpu->r[Vx] = p_cpu->r[Vx] | p_cpu->r[Vy];
}


/*
8xy2 - AND Vx, Vy
Set Vx = Vx AND Vy.

Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, then the same bit in the result is also 1. Otherwise, it is 0.
*/
void AND(int8_t Vx, int8_t Vy)
{
    p_cpu->r[Vx] = p_cpu->r[Vx] & p_cpu->r[Vy];
}

/*
8xy3 - XOR Vx, Vy
Set Vx = Vx XOR Vy.

Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0. 
*/

void XOR(int8_t Vx, int8_t Vy)
{
    p_cpu->r[Vx] = p_cpu->r[Vx] ^ p_cpu->r[Vy];
}

/*
8xy4 - ADD Vx, Vy
Set Vx = Vx + Vy, set VF = carry.

The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
*/
void ADD(int8_t Vx, int8_t Vy) {
    int16_t result = p_cpu->r[Vx] + p_cpu->r[Vy];
    p_cpu->r[Vx] = result & 0xFF;
    p_cpu->r[0xF] = (result > 0xFF);
}

/*
8xy5 - SUB Vx, Vy
Set Vx = Vx - Vy, set VF = NOT borrow.

If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
*/
void SUB(int8_t Vx, int8_t Vy) 
{
    p_cpu->r[0xF] = (Vx > Vy);
    p_cpu->r[Vx] -= p_cpu->r[Vy];
}

void SNE(int8_t Vx, int8_t Vy) {
    if(p_cpu->r[Vx] != p_cpu->r[Vy]) p_cpu->pc += 2;
}

/*
Annn - LD I, addr
Set I = nnn.

The value of register I is set to nnn.
*/

void I_LD(int16_t nnn) {
    p_cpu->ir = nnn;
}