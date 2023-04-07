# $< = input | $@ = output

#TODO: Precompile header files

# Future: $(shell find . -name '*.c')

C_SOURCES = $(wildcard kernel/*.c kernel/drivers/*.c)
ASM_SOURCES = $(wildcard boot/*.asm)

C_OBJS = ${C_SOURCES:.c=.o}
ASM_OBJS = ${ASM_SOURCES:.asm=.o}

# TODO:  | Build cross-compiler so I dont need this...                                                               | I will still need this
CFLAGS = -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I . -I ./include/
ASMFLAGS = -felf32

all: $(C_OBJS) $(ASM_OBJS) link mboot buildiso run clean

$(C_OBJS): $(C_SOURCES)
	$(info [c] $@)
	gcc $(CFLAGS) -c $< -o $@

$(ASM_OBJS): $(ASM_SOURCES)
	$(info [asm] $@)
	nasm $(ASMFLAGS) $< -o $@

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
	qemu-system-i386 -kernel build/kernel.bin
#	qemu-system-i386 -cdrom build/image/BlinkKernel.iso

clean:
	rm -f $(C_OBJS)
	rm -f $(ASM_OBJS)