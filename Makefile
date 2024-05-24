.PHONY: all clean re test

KEY = c

DEF_KEYS = -D "KEY='$(KEY)'"

CFLAGS = -g 

BUILD_DIR = bin
SOURCE_DIR = src

TARGET = cintegrator

all: $(BUILD_DIR) $(TARGET)

re: clean all

clean:
	@rm -f $(BUILD_DIR)/*.o 

test: $(TEST_FILE)
	@echo "[*] Testing..."
	@$(SOURCE_DIR)/test.sh $(TEST_FILE)
	@echo "[+] Testing complete!"

$(BUILD_DIR):
	mkdir -p $@

$(TARGET): $(BUILD_DIR)/functions.o $(BUILD_DIR)/calculation.o $(BUILD_DIR)/main.o
	@echo "[*] Linking main program..."
	@gcc -o $(TARGET) $(BUILD_DIR)/main.o $(BUILD_DIR)/functions.o $(BUILD_DIR)/calculation.o -no-pie -m32
	@echo "[+] Linking complete!"

$(BUILD_DIR)/main.o:	$(SOURCE_DIR)/main.c
	@echo "[*] Compiling main program..."
	@gcc -o $(BUILD_DIR)/main.o $(SOURCE_DIR)/main.c -m32 -c $(DEF_KEYS) $(CFLAGS) || { echo "[-] Compilation of main program failed!"; exit 1; }
	@echo "[+] Compilation complete!"

$(BUILD_DIR)/calculation.o: $(SOURCE_DIR)/calculation.c
	@echo "[*] Compiling calculation program..."
	@gcc -o $(BUILD_DIR)/calculation.o $(SOURCE_DIR)/calculation.c -c -m32 $(CFLAGS) || { echo "[-] Compilation of calculation program failed!"; exit 1; }
	@echo "[+] Compilation complete!"

$(BUILD_DIR)/functions.o: $(SOURCE_DIR)/functions.asm
	@echo "[*] Compiling function listing..."
	@nasm -f elf32 -o $(BUILD_DIR)/functions.o $(SOURCE_DIR)/functions.asm || { echo "[-] Compilation of function listing failed!"; exit 1; }
	@echo "[+] Compilation complete!"

$(SOURCE_DIR)/functions.asm: $(BUILD_DIR)/generator $(SPEC_FILE)
	@echo "[*] Generating function listings..."
	@$(BUILD_DIR)/generator
	@echo "[+] Listings generated successfully!"

$(BUILD_DIR)/generator: $(BUILD_DIR)/generator.o $(BUILD_DIR)/stack.o $(BUILD_DIR)/heap.o
	@echo "[*] Linking generator program..."
	@gcc -o $(BUILD_DIR)/generator $(BUILD_DIR)/generator.o $(BUILD_DIR)/stack.o $(BUILD_DIR)/heap.o || { echo "[-] Compilation of generator program failed!"; exit 1; }
	@echo "[+] Linking complete!"

$(BUILD_DIR)/generator.o:	$(SOURCE_DIR)/generator.c
	@echo "[*] Compiling generator object..."
	@gcc -o $(BUILD_DIR)/generator.o -c $(SOURCE_DIR)/generator.c $(DEF_KEYS) $(CFLAGS) || { echo "[-] Compilation of generator object failed!"; exit 1; }
	@echo "[+] Compilation complete!"

$(BUILD_DIR)/heap.o:		$(SOURCE_DIR)/heap.c
	@echo "[*] Compiling heap object..."
	@gcc -o $(BUILD_DIR)/heap.o -c $(SOURCE_DIR)/heap.c $(CFLAGS) || { echo "[-] Compilation of heap object program failed!"; exit 1; }
	@echo "[+] Compilation complete!"

$(BUILD_DIR)/stack.o:	$(SOURCE_DIR)/stack.c
	@echo "[*] Compiling stack object..."
	@gcc -o $(BUILD_DIR)/stack.o -c $(SOURCE_DIR)/stack.c $(CFLAGS) || { echo "[-] Compilation of stack object program failed!"; exit 1; }
	@echo "[+] Compilation complete!"
