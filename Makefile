all: prog

re: clean all

key = c

def_keys = -D "KEY='$(key)'"

cflags = -g 

clean:
	@rm -f *.o generator functions.asm

prog: functions calculation main
	@echo "[*] Linking main program..."
	@gcc -o cintegrator main.o functions.o calculation.o -m32 -no-pie
	@echo "[+] Linking complete!\n"

main:
	@echo "[*] Compiling main program..."
	@gcc -o main.o main.c -c -m32 $(def_keys) $(cflags) || { echo "[-] Compilation of main program failed!"; exit 1; }
	@echo "[+] Compilation complete!\n"

calculation:
	@echo "[*] Compiling calculation program..."
	@gcc -o calculation.o calculation.c -c -m32 $(cflags) || { echo "[-] Compilation of calculation program failed!"; exit 1; }
	@echo "[+] Compilation complete!\n"

functions: generate
	@echo "[*] Compiling function listing..."
	@nasm -f elf32  -o functions.o functions.asm || { echo "[-] Compilation of function listing failed!"; exit 1; }
	@echo "[+] Compilation complete!\n"

generate: generator
	@echo "[*] Generating function listings..."
	@./generator
	@echo "[+] Listings generated successfully!\n"

generator: generator-obj
	@echo "[*] Linking generator program..."
	@gcc -o generator generator.o stack.o heap.o || { echo "[-] Compilation of generator program failed!"; exit 1; }
	@echo "[+] Linking complete!\n"

generator-obj:
	@echo "[*] Compiling generator objects..."
	@gcc -o generator.o -c gen/generator.c $(def_keys) $(cflags) || { echo "[-] Compilation of generator object failed!"; exit 1; }
	@gcc -o stack.o -c gen/stack.c $(cflags) || { echo "[-] Compilation of stack object program failed!"; exit 1; }
	@gcc -o heap.o -c gen/heap.c $(cflags) || { echo "[-] Compilation of heap object program failed!"; exit 1; }
	@echo "[+] Compilation complete!\n"
