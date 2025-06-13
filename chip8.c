#include "include/chip8.h"
#include "include/raylib.h"

#include <stdio.h>

cpu* p_cpu;
const char sprites[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,      // 0
    0x20, 0x60, 0x20, 0x20, 0x70,      // 1
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



//Raylib screen size
const int screenWidth = 1280;
const int screenHeight = 720;

void draw_registers() {
    for (int i = 0; i < 16; i++) {
        char buffer[32];
        sprintf(buffer, "V%01X: 0x%02X", i, p_cpu->r[i]);

        int col = i / 8;
        int row = i % 8;

        int x = screenWidth / 2 + 340 + col * 160;
        int y = screenHeight / 2 - 100 + row * 25;

        DrawText(buffer, x, y, 32, RED);
    }
}

int main(int argc, char *argv[])
{
    char buffer[255];
    if(argc < 2)
    {
        printf("Usage: %s <path>\n", argv[0]);
        return 1;
    }
    init_cpu();
    load_rom(argv[1]);
    InitWindow(screenWidth, screenHeight, "Chip-8");
    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        loop();
        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(screenWidth-305, screenHeight/2-98, 305, 200, LIGHTGRAY );
        draw_registers();
        draw_screen();
        EndDrawing();
    }
    printf("CHIP-8 Emulator started");
    return 0;
}

void load_rom(const char* path) {
    printf("Reading file %s ", path);
    FILE *fptr = fopen(path, "rb");
    if(fptr != NULL)
    {
        fread(p_cpu->ram + 0x200, 1, RAM_SIZE - 0x200, fptr);
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
    p_cpu->draw_flag = 0;

    CLS();
    memset(p_cpu->display, 0, sizeof(p_cpu->display));
    memcpy(((void*)p_cpu->ram)+SPRITE_OFFSET, &sprites, sizeof(sprites));
    return true;
}

void loop() {
    
    p_cpu->ic = 0;

    if(p_cpu->dt > 0) p_cpu->dt--;
    if(p_cpu->st > 0) p_cpu->dt--;
    handleKey();

    while(p_cpu->ic < 16)
    {
        if(p_cpu->draw_flag) draw_screen();
        uint16_t opcode = p_cpu->ram[p_cpu->pc] << 8 | p_cpu->ram[p_cpu->pc+1]; 

        uint8_t n = opcode & 0x000F;
        uint8_t Vx = (opcode >> 8) & 0x000F;
        uint8_t Vy = (opcode >> 4) & 0x000F;
        uint8_t kk = opcode & 0x00FF;
        uint16_t nnn = opcode & 0x0FFF;

        p_cpu->pc += 2;
        p_cpu->ic++;
        printf("Opcode: %x \n", opcode);
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
                //printf("JMP to %x\n", opcode & 0x0FFF);
                JP(nnn);
                break;
            }
            case 0x2000: // 0x2nnn: CALL
            {
                CALL(nnn);
                break;
            }
            case 0x3000:
            {
                bSE(Vx, kk);
                break;
            }
            case 0x4000:
            {
                bSNE(Vx, kk);
                break;
            }
            case 0x5000:
            {
                SE(Vx, Vy);
                break;
            }
            case 0x6000:
            {
                bLD(Vx, kk);
                break;
            }
            case 0x7000:
            {
                bADD(Vx, kk);
                break;
            }
            case 0x8000:
            {
                switch(opcode & 0xF00F)
                {
                    case 0x8001:
                    {
                        OR(Vx, Vy);
                        break;
                    }
                    case 0x8002:
                    {
                        AND(Vx, Vy);
                        break;
                    }
                    case 0x8003:
                    {
                        XOR(Vx, Vy);
                        break;
                    }
                    case 0x8004:
                    {
                        ADD(Vx, Vy);
                        break;
                    }
                    case 0x8005:
                    {
                        SUB(Vx, Vy);
                        break;
                    }

                }
                break;
            }
            case 0xA000:
            {
                I_LD(nnn);
                break;
            }
            case 0xB000:
            {
                JP_V(nnn);
                break;
            }
            case 0xC000:
            {
                RND(Vx, kk);
                break;
            }
            case 0xD000:
            {
                DRW(Vx, Vy, n);
                break;
            }
            case 0xE000:
            {
                switch(opcode & 0xF0FF)
                {
                    case 0xE09E:
                    {
                        SKP(Vx);
                        break;
                    }
                    case 0xE0A1:
                    {
                        SKNP(Vx);
                        break;
                    }
                }
                break;
            }
            case 0xF000:
            {
                switch(opcode & 0xF0FF)
                {
                    case 0xF007:
                    {
                        LD_VX_DT(Vx);
                        break;
                    }
                    case 0xF00A:
                    {
                        LD_VX_K(Vx);
                        break;
                    }
                    case 0xF015:
                    {
                        LD_DT_VX(Vx);
                        break;
                    }
                    case 0xF018:
                    {
                        LD_ST_VX(Vx);
                        break;
                    }
                    case 0xF0AE:
                    {
                        ADD_I_VX(Vx);
                        break;
                    }
                    case 0xF029:
                    {
                        LD_F_VX(Vx);
                        break;
                    }
                    case 0xF033:
                    {
                        LD_B_VX(Vx);
                        break;
                    }
                    case 0xF055:
                    {
                        LD_I_VX(Vx);
                        break;
                    }
                    case 0xF065:
                    {
                        LD_VX_I(Vx);
                        break;
                    }
                }
                break;
            }
            default:
                printf("Unimplemented 0x%x", opcode);
        }
    }
}

void draw_screen() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            if ((p_cpu->display[y][x]) == 1) {
                int px = (screenWidth / 2) - (SCREEN_WIDTH * SCREEN_SCALE) / 2 + x * SCREEN_SCALE;
                int py = (screenHeight / 2) - (SCREEN_HEIGHT * SCREEN_SCALE) / 2 + y * SCREEN_SCALE;
                DrawRectangle(px, py, SCREEN_SCALE, SCREEN_SCALE, WHITE);
            }
        }
    }
    p_cpu->draw_flag = 0;
}

void handleKey()
{
    p_cpu->keypad[0]  = IsKeyDown(KEY_ONE);
    p_cpu->keypad[1]  = IsKeyDown(KEY_A);
    p_cpu->keypad[2]  = IsKeyDown(KEY_Q);
    p_cpu->keypad[3]  = IsKeyDown(KEY_W);
    p_cpu->keypad[4]  = IsKeyDown(KEY_TWO);
    p_cpu->keypad[5]  = IsKeyDown(KEY_Z);
    p_cpu->keypad[6]  = IsKeyDown(KEY_S);
    p_cpu->keypad[7]  = IsKeyDown(KEY_X);
    p_cpu->keypad[8]  = IsKeyDown(KEY_THREE);
    p_cpu->keypad[9]  = IsKeyDown(KEY_E);
    p_cpu->keypad[10] = IsKeyDown(KEY_D);
    p_cpu->keypad[11] = IsKeyDown(KEY_C);
    p_cpu->keypad[12] = IsKeyDown(KEY_FOUR);
    p_cpu->keypad[13] = IsKeyDown(KEY_R);
    p_cpu->keypad[14] = IsKeyDown(KEY_F);
    p_cpu->keypad[15] = IsKeyDown(KEY_V);
}