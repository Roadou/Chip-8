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

#define STACK_SIZE 16
#define RAM_SIZE 4096
#define NUM_REGISTERS 16
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32


struct s_cpu {

    int16_t pc; // Program Counter
    int8_t sp; // Stack pointer
    
    
    int16_t stack[STACK_SIZE]; // Chip-8 stack is 16
    int8_t ram[RAM_SIZE]; // RAM (4kb), most programs start at 0x200
    int8_t r[NUM_REGISTERS]; // Chip-8 have 16 x 8 bits registers
    int16_t ir; // I register

    int8_t st; // Sound timer, always beep if > 0
    int8_t dt; // Delay timer

    int16_t ic; // Instruction counter

    char display[SCREEN_WIDTH][SCREEN_HEIGHT];

    bool draw_flag;
};

typedef struct s_cpu cpu;

extern cpu *p_cpu;

void JP(int16_t);
void CLS();
void SE(int8_t, int8_t);
void SNE(int8_t, int8_t);
void CALL(int16_t);
void RET();
void bSE(int8_t Vx, int8_t kk);
void bSNE(int8_t Vx, int8_t kk);

#endif
