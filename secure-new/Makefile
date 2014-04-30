.phony: all

all: secure.bin

CC = /home/shenchun/iauto/prebuilt/toolchains/arm-fsl-linux-gnueabi/4.6.2/bin/arm-none-linux-gnueabi-gcc
LD = /home/shenchun/iauto/prebuilt/toolchains/arm-fsl-linux-gnueabi/4.6.2/bin/arm-none-linux-gnueabi-ld
OBJ = /home/shenchun/iauto/prebuilt/toolchains/arm-fsl-linux-gnueabi/4.6.2/bin/arm-none-linux-gnueabi-objcopy
DUMP= /home/shenchun/iauto/prebuilt/toolchains/arm-fsl-linux-gnueabi/4.6.2/bin/arm-none-linux-gnueabi-objdump

secure.bin: secure.elf
	$(OBJ) -O binary secure.elf secure-origin.bin
	$(DUMP) -D secure.elf > dump_secure.log
	$(OBJ) -I binary -O binary --pad-to 0x1000 --gap-fill=0x00 secure-origin.bin secure.bin
	
secure.elf: flash_header.S monitor.S secure_startup.S secure_main.c  
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -nostdlib flash_header.S
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -nostdlib monitor.S
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -nostdlib secure_startup.S
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -nostdlib secure_main.c
	$(LD) -T secure.lds -o secure.elf flash_header.o monitor.o secure_startup.o secure_main.o  
	
clean:
	$(RM) *.o *.i *.bin
	$(RM) *.elf *~ *.log
	
	
