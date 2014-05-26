.phony: all

all: secure.bin

CC  = arm-none-linux-gnueabi-gcc
LD  = arm-none-linux-gnueabi-ld
OBJ = arm-none-linux-gnueabi-objcopy
DUMP= arm-none-linux-gnueabi-objdump

secure.bin: secure.elf
	$(OBJ) -O binary secure.elf secure-origin.bin
	$(DUMP) -D secure.elf > dump_secure.log
	$(OBJ) -I binary -O binary --pad-to 0x1000 --gap-fill=0x00 secure-origin.bin secure.bin
	
secure.elf: flash_header.S monitor.S secure_startup.S secure_main.c  
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -Os -nostdlib flash_header.S
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -Os -nostdlib monitor.S
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -Os -nostdlib secure_startup.S
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -Os -nostdlib secure_main.c
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -Os -nostdlib uart.c
	$(LD) -T secure.lds -o secure.elf flash_header.o monitor.o secure_startup.o secure_main.o uart.o 
	
clean:
	$(RM) *.o *.i *.bin
	$(RM) *.elf *~ *.log
	
	
