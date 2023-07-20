nasm -felf32 ./boot/entry.asm -o ./bin/entry.o

i686-elf-gcc -c ./kernel/kernel.c -o ./bin/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

i686-elf-ld -T link.ld -o ./bin/kernel.bin ./bin/entry.o ./bin/kernel.o

qemu-system-x86_64 -kernel ./bin/kernel.bin