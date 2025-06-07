all: chip-8

mymalloc:chip8.c
	gcc -Wall -Wextra -Wformat=0 -D_LOG_ALL -fsanitize=address -Iinclude chip8.c  -o chip8.out
	#gcc -Wall -Wextra -fPIC -shared malloc2.c -o malloc2.so
	