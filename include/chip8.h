#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#ifndef CHIP8_H
#define CHIP8_H

//Contains CHIP-8 defualt sprites located in the interpreter area [0x0-0x200]
const int16_t sprites[0xF][5] = {
    {0xF0, 0x90, 0x90, 0x90, 0xF0}, // 0
    {0x20, 0x60, 0x20, 0x20, 0x70}, // 1
    {0xF0, 0x10, 0xF0, 0x80, 0xF0}, // 2
    {0xF0, 0x10, 0xF0, 0x10, 0xF0}, // 3
};


struct s_cpu {

    int16_t pc; // Program Counter
    int8_t sp; // Stack pointer
    
    int16_t stack[16]; // Chip-8 stack is 16
    int8_t ram[0x1000]; // RAM (4kb), most programs start at 0x200
    int8_t r[16]; // Chip-8 have 16 x 8 bits registers

    int8_t st; // Sound timer, always beep if > 0
    int8_t dt; // Delay timer

    int16_t ic; // Instruction counter

    char display[16][8];
};

typedef struct s_cpu cpu;

extern cpu *p_cpu;


// Instructions
void JP(int16_t);
void CLS();
void SE(int8_t, int8_t);
void SNE(int8_t, int8_t);
void CALL(int16_t);
void RET();

#endif
