all: chip-8

chip-8:chip8.c
	gcc -Wall -Wextra -Wformat=0 -D_LOG_ALL -fsanitize=address -Iinclude chip8.c instructions.c -o chip8.out -lraylib -Llib	