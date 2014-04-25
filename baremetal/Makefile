.phony: all

all: test.bin

CC = /home/shenchun/iauto/prebuilt/toolchains/arm-fsl-linux-gnueabi/4.6.2/bin/arm-none-linux-gnueabi-gcc
LD = /home/shenchun/iauto/prebuilt/toolchains/arm-fsl-linux-gnueabi/4.6.2/bin/arm-none-linux-gnueabi-ld
OBJ = /home/shenchun/iauto/prebuilt/toolchains/arm-fsl-linux-gnueabi/4.6.2/bin/arm-none-linux-gnueabi-objcopy
DUMP= /home/shenchun/iauto/prebuilt/toolchains/arm-fsl-linux-gnueabi/4.6.2/bin/arm-none-linux-gnueabi-objdump

test.bin: test.elf
	$(OBJ) -O binary test.elf test.bin
	$(DUMP) -D test.elf > dump.log

test.elf: flash_header.S start.S main.c uart.c iomux-v3.c linker.lds
#	$(CC)   -D__ASSEMBLY__ -g  -Os   -fno-strict-aliasing -fno-common -ffixed-r8 -msoft-float -D__KERNEL__ -DTEXT_BASE=0x00907000 -fno-builtin -ffreestanding -nostdinc -isystem /home/shenchun/arm2010q1-202/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-linux-gnueabi/4.4.1/include -pipe  -DCONFIG_ARM -D__ARM__ -marm  -mabi=aapcs-linux -mno-thumb-interwork -march=armv5     -o flash_header.o flash_header.S -c
	
#	$(CC)   -D__ASSEMBLY__ -g  -Os   -fno-strict-aliasing -fno-common -ffixed-r8 -msoft-float -D__KERNEL__ -DTEXT_BASE=0x00907000 -fno-builtin -ffreestanding -nostdinc -isystem /home/shenchun/arm2010q1-202/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-linux-gnueabi/4.4.1/include -pipe  -DCONFIG_ARM -D__ARM__ -marm  -mabi=aapcs-linux -mno-thumb-interwork -march=armv5     -o start.o start.S -c
	
#	$(LD) -Bstatic -T linker.lds -o test.elf flash_header.o start.o  --print-map

	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -nostdlib flash_header.S
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -nostdlib start.S
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -nostdlib main.c
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -nostdlib uart.c
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -nostdlib iomux-v3.c
	$(LD) -T linker.lds -o test.elf start.o main.o uart.o iomux-v3.o flash_header.o
	
clean:
	$(RM) *.o *.i *.bin
	$(RM) *.elf *~ *.log
	
	
