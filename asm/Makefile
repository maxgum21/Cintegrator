all: test loader
	gcc -m32 -o test test.o loader.o

clean:
	rm -r test *.o

test:
	nasm -f elf32 -o test.o test.asm

loader:
	gcc -m32 -o loader.o loader.c -c -g

re: clean all
