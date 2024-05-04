all: prog

re: clean all

key = c

def_keys = -D "KEY='$(key)'"

cflags = -g 

clean:
	@rm *.o generator functions.asm

prog: functions main
	@gcc -o cintegrator main.o functions.o -m32 -no-pie

main:
	@echo "[*] Compiling main program..."
	@gcc -o main.o main.c -c -m32 $(def_keys) $(cflags) || { echo "[-] Compilation of main program failed!"; exit 1; }
	@echo "[+] Compilation complete!"

functions: generate
	@echo "[*] Compling function listing..."
	@nasm -f elf32  -o functions.o functions.asm || { echo "[-] Compilation of function listing failed!"; exit 1; }
	@echo "[+] Compilation complete!"

generate: generator
	@echo "[*] Generating function listings..."
	@./generator
	@echo "[+] Listings generated successfully!"

generator: generator-obj
	@echo "[*] Linking generator program..."
	@gcc -o generator generator.o stack.o heap.o || { echo "[-] Compilation of generator program failed!"; exit 1; }
	@echo "[+] Generator program compiled successfully!"

generator-obj:
	@echo "[*] Compiling generator program objects..."
	@gcc -o generator.o -c gen/generator.c $(def_keys) $(cflags) || { echo "[-] Compilation of generator object failed!"; exit 1; }
	@gcc -o stack.o -c gen/stack.c $(cflags) || { echo "[-] Compilation of stack object program failed!"; exit 1; }
	@gcc -o heap.o -c gen/heap.c $(cflags) || { echo "[-] Compilation of heap object program failed!"; exit 1; }
	@echo "[+] Compiltaion of generator program objects"
