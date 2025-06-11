#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "raylib.h"

#ifndef CHIP8_H
#define CHIP8_H

#define ENABLED_PIXEL "█"
#define DISABLED_PIXEL " "
//Contains CHIP-8 defualt sprites located in the interpreter area [0x0-0x200]
extern const char sprites[];
#define STACK_SIZE 16
#define RAM_SIZE 4096
#define NUM_REGISTERS 16
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define SCREEN_SCALE 8

struct s_cpu {

    int16_t pc; // Program Counter
    int8_t sp; // Stack pointer
    
    
    int16_t stack[STACK_SIZE]; // Chip-8 stack is 16
    uint8_t ram[RAM_SIZE]; // RAM (4kb), most programs start at 0x200
    int8_t r[NUM_REGISTERS]; // Chip-8 have 16 x 8 bits registers
    int16_t ir; // I register

    int8_t st; // Sound timer, always beep if > 0
    int8_t dt; // Delay timer

    int16_t ic; // Instruction counter

    char display[SCREEN_HEIGHT][SCREEN_WIDTH];

    bool draw_flag;
};

typedef struct s_cpu cpu;

extern cpu *p_cpu;

bool init_cpu();
void load_rom(const char*);
void loop();
void draw_screen();


void JP(int16_t);
void CLS();
void SE(uint8_t Vx, uint8_t Vy);
void SNE(uint8_t Vx, uint8_t Vy);
void CALL(int16_t);
void RET();
void bSE(uint8_t Vx, int8_t kk);
void bSNE(uint8_t Vx, int8_t kk);
void bLD(uint8_t Vx, int8_t kk);
void bADD(uint8_t Vx, int8_t kk);
void OR(uint8_t Vx, uint8_t Vy);
void AND(uint8_t Vx, uint8_t Vy);
void XOR(uint8_t Vx, uint8_t Vy);
void ADD(uint8_t Vx, uint8_t Vy);
void SUB(uint8_t Vx, uint8_t Vy);
void SHR(uint8_t Vx);
void SUBN(uint8_t Vx, uint8_t Vy);
void SHL(uint8_t Vx);
void SNE(uint8_t Vx, uint8_t Vy);
void I_LD(int16_t addr);
void JP_V(int16_t addr);
void DRW(uint8_t Vx, uint8_t Vy, uint8_t n);

#endif
