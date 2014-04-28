.phony: all

all: test.bin

CC = /home/shenchun/iauto/prebuilt/toolchains/arm-fsl-linux-gnueabi/4.6.2/bin/arm-none-linux-gnueabi-gcc
LD = /home/shenchun/iauto/prebuilt/toolchains/arm-fsl-linux-gnueabi/4.6.2/bin/arm-none-linux-gnueabi-ld
OBJ = /home/shenchun/iauto/prebuilt/toolchains/arm-fsl-linux-gnueabi/4.6.2/bin/arm-none-linux-gnueabi-objcopy
DUMP= /home/shenchun/iauto/prebuilt/toolchains/arm-fsl-linux-gnueabi/4.6.2/bin/arm-none-linux-gnueabi-objdump

test.bin: test.elf
	$(OBJ) -O binary test.elf test.bin

test.elf: start.S test.c
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -nostdlib start.S
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -nostdlib test.c
	$(LD) -T test.lds -o test.elf start.o test.o
	
clean:
	$(RM) *.o *.i *.bin
	$(RM) *.elf *~ *.log
	
	
