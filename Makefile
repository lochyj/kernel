export
# $< = input | $@ = output

#TODO: Precompile header files

C_SOURCES = $(wildcard kernel/*.c)
ASM_SOURCES = $(wildcard boot/*.asm)

C_OBJS = ${C_SOURCES:.c=.o}
ASM_OBJS = ${ASM_FILES:.asm=.o}

# TODO:  | Build cross-compiler so I dont need this...                                                               | I will still need this
CFLAGS = -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I . -I ./include/
ASMFLAGS = -felf32

all: link mboot buildiso run clean

%.o: %.asm
	nasm $(ASMFLAGS) $< -o $@

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

link: ${ASM_OBJS} ${C_OBJS}
	ld -m elf_i386 -T linker.ld -o build/kernel.bin $^

mboot: build/kernel.bin
	grub2-file --is-x86-multiboot $<

buildiso:
	mkdir -p build/iso/boot/grub
	cp build/kernel.bin build/iso/boot/kernel.bin
	cp grub.cfg build/iso/boot/grub/grub.cfg
	grub-mkrescue -o build/image/BlinkKernel.iso build/iso

run:
	qemu-system-i386 -cdrom build/image/BlinkKernel.iso

clean:
	rm -f $(C_OBJS)
	rm -f $(ASM_OBJS)