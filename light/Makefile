# $< = input | $@ = output

# Future: $(shell find . -name '*.c')

C_SOURCES = $(wildcard kernel/*.c kernel/irq/*.c kernel/time/*.c \
				drivers/display/fbdev/*.c drivers/display/vga/*.c drivers/input/keyboard/*.c drivers/input/mouse/*.c drivers/input/display/fbdev/fonts/*.c \
				lib/math/*.c lib/string/*.c lib/stdio/*.c \
				misc/debug/*.c misc/vm/*.c \
				mm/*.c \
				fs/*.c \
				)

ASM_SOURCES = $(wildcard kernel/*.asm  kernel/irq/*.asm \
				mm/*.asm \
				)

C_OBJS = ${C_SOURCES:.c=.o}
ASM_OBJS = ${ASM_SOURCES:.asm=.out}

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Wno-int-conversion -Wno-unused-parameter -Wno-unused-function -I . -I ./include/ -I ./include/light/lib/
ASMFLAGS = -felf32
BUILD_DIR = ../build

.PHONY: all link mboot buildiso run build clean

all: $(ASM_OBJS) $(C_OBJS) link mboot buildiso run clean

%.out: %.asm
	nasm $(ASMFLAGS) $< -o $@

%.o: %.c
	i686-elf-gcc $(CFLAGS) -c $< -o $@

link:
	i686-elf-ld -T linker.ld $(ASM_OBJS) $(C_OBJS) -o $(BUILD_DIR)/light.bin

mboot: $(BUILD_DIR)/light.bin
	grub2-file --is-x86-multiboot $<

buildiso:
	mkdir -p $(BUILD_DIR)/iso/boot/grub

	cp $(BUILD_DIR)/light.bin $(BUILD_DIR)/iso/boot/light.bin
	cp grub.cfg $(BUILD_DIR)/iso/boot/grub/grub.cfg

	grub2-mkrescue -o $(BUILD_DIR)/image/BlinkOS.iso $(BUILD_DIR)/iso

run:

#	This is used for testing
#	qemu-system-i386 -kernel build/light.bin
#	qemu-system-i386 -cdrom build/image/BlinkOS.iso -monitor stdio

# 	Please use this if you want to try out the kernel
	qemu-system-i386                                 	\
		-accel tcg,thread=single                       	\
		-cpu core2duo                                  	\
		-m 128                                         	\
		-drive format=raw,media=cdrom,file=$(BUILD_DIR)/image/BlinkOS.iso\
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
	rm -f $(BUILD_DIR)/light.bin