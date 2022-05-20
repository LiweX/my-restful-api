CFLAGS=-Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11

all: lab6 contador

lab6: lab6.c
	gcc $(CFLAGS) lab6.c -o lab6 -lulfius -ljansson

contador: contador.c
	gcc $(CFLAGS) contador.c -o contador -lulfius -ljansson