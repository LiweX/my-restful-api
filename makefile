CFLAGS=-Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11

lab6: lab6.c
	gcc $(CFLAGS) lab6.c -o lab6 -lulfius -ljansson