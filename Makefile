# $< = input | $@ = output

# Future: $(shell find . -name '*.c')

.PHONY: all kernel mboot buildiso run clean

all: kernel
# future: add below to "all:"
# mboot buildiso run clean

kernel:
#	Change to the kernel directory for simplicity
	cd light && make

# 	Return to root directory
	cd ../

mboot: build/kernel.bin
	grub2-file --is-x86-multiboot $<

buildiso:
	mkdir -p build/iso/boot/grub

	cp build/kernel.bin build/iso/boot/kernel.bin
	cp grub.cfg build/iso/boot/grub/grub.cfg

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

clean:
#	Change to the kernel directory for simplicity
	cd light && make clean

# 	Return to root directory
	cd ../