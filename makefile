CFLAGS=-Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11

all: lab6 contador

lab6: lab6.c tools.o
	gcc $(CFLAGS) lab6.c -o lab6 tools.o -lulfius -ljansson

contador: contador.c tools.o
	gcc $(CFLAGS) contador.c -o contador tools.o -lulfius -ljansson

tools.o: tools.c tools.h
	gcc $(CFLAGS) -c tools.c 