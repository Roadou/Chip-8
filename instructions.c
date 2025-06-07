#include "include/chip8.h"

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
1nnn - JP addr
Jump to location nnn.

The interpreter sets the program counter to nnn.
*/
void JP(int16_t addr)
{
    p_cpu->pc = addr;
}

/*
00E0 - CLS
Clear the display.
*/
void CLS()
{
    memset(p_cpu->display, 0, sizeof(p_cpu->display));
}


void SE(int8_t Vx, int8_t Vy) {
    if(p_cpu->r[Vx] == p_cpu->r[Vy]) p_cpu->pc += 2;
}

void SNE(int8_t Vx, int8_t Vy) {
    if(p_cpu->r[Vx] != p_cpu->r[Vy]) p_cpu->pc += 2;
}

void bSE(int8_t Vx, int8_t kk)
{
    if(p_cpu->r[Vx] == kk) p_cpu->pc += 2;
}

void bSNE(int8_t Vx, int8_t kk)
{
    if(p_cpu->r[Vx] != kk) p_cpu->pc += 2;
}

void CALL(int16_t addr)
{
    p_cpu->pc++;
}
