# Variables
CC = x86_64-elf-gcc
CFLAGS = -m32 -std=gnu99 -ffreestanding -Wall -Wextra -fno-pic
LD = x86_64-elf-ld
LDFLAGS = -m elf_i386 -T linker.ld
OBJCOPY = x86_64-elf-objcopy
NASM = nasm
NASMFLAGS = -f elf32

# Default target
all: boot.bin

# Link
boot.elf: bootloader.o kernel.o linker.ld
	$(LD) $(LDFLAGS) bootloader.o kernel.o -o boot.elf

boot.bin: boot.elf
	$(OBJCOPY) -O binary boot.elf boot.bin

# Compile C
kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

# Compile ASM
bootloader.o: bootloader.asm
	$(NASM) $(NASMFLAGS) bootloader.asm -o bootloader.o

# Clean
clean:
	rm -f *.o boot.bin
