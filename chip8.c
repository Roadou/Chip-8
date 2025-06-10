#include "include/chip8.h"
#include <stdio.h>

cpu* p_cpu;

int main(int argc, char *argv[])
{
    char buffer[255];
    if(argc > 0)
    {
        load_rom(argv[0]);
    }
    printf("CHIP-8 Emulator started");
    return 0;
}

void load_rom(const char* path) {
    FILE *fptr = fopen(path, "r");
    if(fptr != NULL)
    {
        fgets(p_cpu->ram, RAM_SIZE, fptr);
        fclose(fptr);
        printf("Loaded %s into memory", path);
    }
}

bool init_cpu() {

    p_cpu = (void*)malloc(sizeof(cpu));
    p_cpu->pc = 0x200;
    p_cpu->sp = 0x0;
    p_cpu->ic = 0;
    p_cpu->st = 0;
    p_cpu->dt = 0;

    CLS();
    memset(p_cpu->display, 0, sizeof(p_cpu->display));
    memcpy(((void*)p_cpu->ram)+0x50, &sprites, sizeof(sprites));
    return true;
}

void loop() {
    
    while(true)
    {
        uint16_t opcode = p_cpu->ram[p_cpu->pc] << 8 | p_cpu->ram[p_cpu->pc+1]; 
        
        switch(opcode & 0xF000) {
            case 0x0000:
            {
                switch(opcode & 0x00FF) {
                    case 0x00E0: // 00E0: CLS
                        CLS();
                        break;
                    case 0x00EE: // 0x00EE: RET
                        RET();
                        break;
                }
                break;
            }
            case 0x1000: // 0x1nnn: JP
            {
                JP(opcode & 0x0FFF);
                break;
            }
            case 0x2000: // 0x2nnn: CALL
            {
                CALL(opcode & 0x0FFF);
                break;
            }
            case 0x3000:
            {
                bSE((opcode >> 8) & 0x0F, opcode & 0x00FF);
                break;
            }
            case 0x4000:
            {
                bSNE((opcode >> 8) & 0x0F, opcode & 0x00FF);
                break;
            }
            case 0x5000:
            {
                SE((opcode >> 8) & 0x000F, (opcode >> 4) & 0x000F);
                break;
            }
            case 0x6000:
            {
                bLD((opcode >> 8) & 0x000F, opcode & 0x00FF);
                break;
            }
            case 0x7000:
            {
                bADD((opcode >> 8) & 0x000F, opcode & 0x00FF);
                break;
            }
            case 0x8000:
            {
                switch(opcode & 0xF00F)
                {
                    case 0x8001:
                    {
                        OR((opcode >> 8) & 0x000F, (opcode >> 4) & 0x000F);
                        break;
                    }
                    case 0x8002:
                    {
                        AND((opcode >> 8) & 0x000F, (opcode >> 4) & 0x000F);
                        break;
                    }
                    case 0x8003:
                    {
                        XOR((opcode >> 8) & 0x000F, (opcode >> 4) & 0x000F);
                        break;
                    }
                    case 0x8004:
                    {
                        ADD((opcode >> 8) & 0x000F, (opcode >> 4) & 0x000F);
                        break;
                    }
                    case 0x8005:
                    {
                        SUB((opcode >> 8) & 0x000F, (opcode >> 4) & 0x000F);
                        break;
                    }

                }
                break;
            }
            case 0xA000:
            {
                I_LD(opcode & 0x0FFF);
                break;
            }
            case 0xB000:
            {
                JP_V(opcode & 0x0FFF);
                break;
            }
        }
    }
}