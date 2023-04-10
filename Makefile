# $< = input | $@ = output

#TODO: Precompile header files

# Future: $(shell find . -name '*.c')

C_SOURCES = $(wildcard kernel/*.c kernel/drivers/*.c lib/libc/string/*.c lib/libc/stdio/*.c kernel/system/gdt/*.c kernel/system/interrupts/*.c kernel/system/*.c)
ASM_SOURCES = $(wildcard boot/*.asm kernel/system/gdt/*.asm kernel/system/assemblies/*.asm kernel/system/interrupts/*.asm)

C_OBJS = ${C_SOURCES:.c=.o}
ASM_OBJS = ${ASM_SOURCES:.asm=.out}

# TODO:  | Build cross-compiler so I dont need this...                                                               | I will still need this
CFLAGS = -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I . -I ./include/
ASMFLAGS = -felf32

all: $(ASM_OBJS) $(C_OBJS) link mboot buildiso run clean

%.out: %.asm
	nasm $(ASMFLAGS) $< -o $@

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

link:
	ld -m elf_i386 -T linker.ld $(ASM_OBJS) $(C_OBJS) -o build/kernel.bin

mboot: build/kernel.bin
	grub2-file --is-x86-multiboot $<

buildiso:
	mkdir -p build/iso/boot/grub
	cp build/kernel.bin build/iso/boot/kernel.bin
	cp grub.cfg build/iso/boot/grub/grub.cfg
	grub2-mkrescue -o build/image/BlinkKernel.iso build/iso

run:
#	qemu-system-i386 -kernel build/kernel.bin
	qemu-system-i386 -cdrom build/image/BlinkKernel.iso -monitor stdio

clean:
	rm -f $(C_OBJS)
	rm -f $(ASM_OBJS)
	rm -f build/kernel.bin