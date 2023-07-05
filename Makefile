# Variables
CC = gcc
CFLAGS = -m16 -march=i686 -O0 -ffreestanding -static -nostdlib -mmanual-endbr -Wall -Wextra -fno-pic
LD = ld
LDFLAGS = -m elf_i386 -static -T link_script.ld -nostdlib --nmagic
OBJCOPY = objcopy
BUILDDIR = build

# Custom build settings. build_settings.mk file is optional
ifeq ($(wildcard build_settings.mk),build_settings.mk)
    include build_settings.mk
endif

# Default target
all: $(BUILDDIR) $(BUILDDIR)/mbr_program.bin

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Compile
$(BUILDDIR)/mbr_program.o: mbr_program.c
	$(CC) $(CFLAGS) -c mbr_program.c -o $(BUILDDIR)/mbr_program.o

# Link
$(BUILDDIR)/mbr_program.elf: $(BUILDDIR)/mbr_program.o link_script.ld
	$(LD) $(LDFLAGS) $(BUILDDIR)/mbr_program.o -o $(BUILDDIR)/mbr_program.elf

# Objcopy
$(BUILDDIR)/mbr_program.bin: $(BUILDDIR)/mbr_program.elf
	$(OBJCOPY) -O binary $(BUILDDIR)/mbr_program.elf $(BUILDDIR)/mbr_program.bin

# Clean
clean:
	rm -rf $(BUILDDIR)
