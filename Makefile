# $< = input | $@ = output

# Future: $(shell find . -name '*.c')

C_SOURCES = $(wildcard kernel/*.c lib/blink/*.c kernel/system/debug/*.c kernel/drivers/*.c lib/libc/string/*.c lib/libc/stdio/*.c kernel/system/memory/*.c kernel/system/interrupts/*.c kernel/system/pit/*.c)
ASM_SOURCES = $(wildcard asm/*.asm)

APPS_ASM_SOURCES = $(wildcard apps/*.asm)
APPS_C_SOURCES = $(wildcard apps/*.c)

C_OBJS = ${C_SOURCES:.c=.o}
ASM_OBJS = ${ASM_SOURCES:.asm=.out}

APPS_C_OBJS = ${APPS_C_SOURCES:.c=.o}
APPS_ASM_OBJS = ${APPS_ASM_SOURCES:.asm=.out}

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Wno-int-conversion -Wno-unused-parameter -Wno-unused-function -I . -I ./include/ -I ./include/lib/
ASMFLAGS = -felf32

.PHONY: all apps link mboot buildiso run build clean

all: $(ASM_OBJS) $(C_OBJS) apps link mboot buildiso run clean

%.out: %.asm
	nasm $(ASMFLAGS) $< -o $@

%.o: %.c
	i686-elf-gcc $(CFLAGS) -c $< -o $@

apps:
	nasm -f bin ./apps/simple.asm -o ./build/app

link:
	i686-elf-ld -T linker.ld $(ASM_OBJS) $(C_OBJS) -o build/kernel.bin

mboot: build/kernel.bin
	grub2-file --is-x86-multiboot $<

buildiso:
	mkdir -p build/iso/boot/grub
	mkdir -p build/iso/modules

	cp build/kernel.bin build/iso/boot/kernel.bin
	cp grub.cfg build/iso/boot/grub/grub.cfg

	cp build/app build/iso/modules/app

	grub2-mkrescue -o build/image/BlinkOS.iso build/iso

run:

#	This is used for testing
#	qemu-system-i386 -kernel build/kernel.bin
#	qemu-system-i386 -cdrom build/image/BlinkOS.iso -monitor stdio

# 	Please use this if you want to try out the kernel
	qemu-system-i386                                 	\
		-accel tcg,thread=single                       	\
		-cpu core2duo                                  	\
		-m 128                                         	\
		-drive format=raw,media=cdrom,file=build/image/BlinkOS.iso\
		-monitor stdio                                 	\
		-smp 1                                         	\
		-usb                                           	\
		-vga std										\
		-d int -no-shutdown -no-reboot
#		-no-reboot                                     	\

build: $(ASM_OBJS) $(C_OBJS) link mboot buildiso run

clean:
	rm -f $(C_OBJS)
	rm -f $(ASM_OBJS)
	rm -f build/kernel.bin