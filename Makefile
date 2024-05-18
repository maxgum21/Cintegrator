all: cintegrator

re: clean all

key = c

def_keys = -D "KEY='$(key)'"

cflags = -g 

spec = $(SPEC_FILE)

clean:
	@rm -f *.o generator functions.asm

cintegrator: functions.o calculation.o main.o
	@echo "[*] Linking main program..."
	@gcc -o cintegrator main.o functions.o calculation.o -no-pie -m32
	@echo "[+] Linking complete!\n"

main.o:
	@echo "[*] Compiling main program..."
	@gcc -o main.o main.c -m32 -c $(def_keys) $(cflags) || { echo "[-] Compilation of main program failed!"; exit 1; }
	@echo "[+] Compilation complete!\n"

calculation.o:
	@echo "[*] Compiling calculation program..."
	@gcc -o calculation.o calculation.c -c -m32 $(cflags) || { echo "[-] Compilation of calculation program failed!"; exit 1; }
	@echo "[+] Compilation complete!\n"

functions.o: functions.asm
	@echo "[*] Compiling function listing..."
	@nasm -f elf32  -o functions.o functions.asm || { echo "[-] Compilation of function listing failed!"; exit 1; }
	@echo "[+] Compilation complete!\n"

functions.asm: generator $(SPEC_FILE)
	@echo "[*] Generating function listings..."
	@./generator
	@echo "[+] Listings generated successfully!\n"

generator: generator.o
	@echo "[*] Linking generator program..."
	@gcc -o generator generator.o stack.o heap.o || { echo "[-] Compilation of generator program failed!"; exit 1; }
	@echo "[+] Linking complete!\n"

generator.o:
	@echo "[*] Compiling generator objects..."
	@gcc -o generator.o -c gen/generator.c $(def_keys) $(cflags) || { echo "[-] Compilation of generator object failed!"; exit 1; }
	@gcc -o stack.o -c gen/stack.c $(cflags) || { echo "[-] Compilation of stack object program failed!"; exit 1; }
	@gcc -o heap.o -c gen/heap.c $(cflags) || { echo "[-] Compilation of heap object program failed!"; exit 1; }
	@echo "[+] Compilation complete!\n"

