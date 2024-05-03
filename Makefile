all: prog 	

re: clean all

key = 't'

cflags = -g -m32

prog: main functions
	gcc -o cintegrator main.o functions.o -lm -g -D KEY=$(key)

clean:
	rm *.o generator

functions: generate
	nasm -elf32 -o functions.o functions.asm

generate: generator
	./generator

generator: generator-obj
	gcc -o generator generator.o stack.o heap.o

generator-obj:
	gcc -o generator.o -c gen/generator.c $(cflags)
	gcc -o stack.o -c gen/stack.c $(cflags)
	gcc -o heap.o -c gen/heap.c $(cflags)
