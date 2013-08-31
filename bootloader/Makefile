all: 
	nasm bootloader2.s -f bin -o bootloader.bin
	nasm padding.asm -f bin -o padding.bin
	gcc -ffreestanding -c kernel.c -o kernel.o
	ld -o kernel.bin -Ttext 0x1000 kernel.o --oformat=binary
	cat bootloader.bin kernel.bin padding.bin > bootloader.img

bootloader:
	nasm bootloader.s -f bin -o bootloader.bin
	mv bootloader.bin bootloader.img

clean:
	rm -rf *.bin *.o *.img
