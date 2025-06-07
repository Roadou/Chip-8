#include "include/chip8.h"
 

cpu* p_cpu;

int main()
{
    printf("CHIP-8 Emulator started");

    
    return 0;
}

bool init_cpu() {

    p_cpu = (void*)malloc(sizeof(cpu));
    p_cpu->pc = 0x200;
    p_cpu->sp = 0x0;

    CLS();
    memset(p_cpu->display, 0, sizeof(p_cpu->display));
    memcpy(p_cpu->ram, &sprites, sizeof(sprites));
    return true;
}

void loop() {
    
    while(true)
    {
        uint16_t opcode = p_cpu->ram[p_cpu->sp] << 8 | p_cpu->ram[p_cpu->sp+1]; 
        
        switch(opcode & 0xF000) {
            case 0x0000:
                switch(opcode & 0x00FF) {
                    case 0x00E0: // 00E0: CLS
                        CLS();
                        break;
                    case 0x00EE: // 0x00EE: RET
                        RET();
                        break;
                }
                break;
            case 0x1000: // 0x1nnn: JP
                {
                    uint16_t addr = opcode & 0x0FFF;
                    JP(addr);
                    break;
                }
            case 0x2000: // 0x2nnn: CALL
                {
                    uint16_t addr = opcode & 0x0FFF;
                    CALL(addr);
                    break;
                }

        }
    }
}