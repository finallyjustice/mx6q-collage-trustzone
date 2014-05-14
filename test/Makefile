.phony: all

all: test.bin

CC  = arm-none-linux-gnueabi-gcc
LD  = arm-none-linux-gnueabi-ld
OBJ = arm-none-linux-gnueabi-objcopy
DUMP= arm-none-linux-gnueabi-objdump

test.bin: test.elf
	$(OBJ) -O binary test.elf test.bin

test.elf: start.S test.c
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -nostdlib start.S
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -nostdlib test.c
	$(LD) -T test.lds -o test.elf start.o test.o
	
clean:
	$(RM) *.o *.i *.bin
	$(RM) *.elf *~ *.log
	
	
