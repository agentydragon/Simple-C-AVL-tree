CC=gcc
CFLAGS=-O2 -Wall -W -pedantic

all: avl_demo

avl_demo:
	$(CC) avl_demo.c avl.c -o avl_demo

clean:
	@rm avl_demo

.PHONY: clean
