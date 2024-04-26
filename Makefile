all: prog		

re: clean prog

prog: main.c
	gcc -o prog main.c -lm -g

clean:
	rm -rf prog
