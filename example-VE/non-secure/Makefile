.phony: all

all: normal.bin

CC = /home/shenchun/iauto/prebuilt/toolchains/arm-fsl-linux-gnueabi/4.6.2/bin/arm-none-linux-gnueabi-gcc
LD = /home/shenchun/iauto/prebuilt/toolchains/arm-fsl-linux-gnueabi/4.6.2/bin/arm-none-linux-gnueabi-ld
OBJ = /home/shenchun/iauto/prebuilt/toolchains/arm-fsl-linux-gnueabi/4.6.2/bin/arm-none-linux-gnueabi-objcopy
DUMP= /home/shenchun/iauto/prebuilt/toolchains/arm-fsl-linux-gnueabi/4.6.2/bin/arm-none-linux-gnueabi-objdump

normal.bin: normal.elf
	$(OBJ) -O binary normal.elf normal.bin
	$(DUMP) -D normal.elf > dump_normal.log

normal.elf: normal_startup.S normal_main.c
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -nostdlib normal_startup.S
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -nostdlib normal_main.c
	$(LD) -T normal.lds -o normal.elf normal_startup.o normal_main.o
	
clean:
	$(RM) *.o *.i *.bin
	$(RM) *.elf *~ *.log
	
	
