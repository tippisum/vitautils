CC = arm-vita-eabi-gcc
CFLAGS  = -g -Wl,-q -Wall -Os -nostdlib -nostartfiles
LIBS = -lSceIofilemgrForDriver_stub -lSceSysclibForDriver_stub -lSceSysmemForDriver_stub



all : tmvMemProbe.skprx

tmvMemProbe.skprx : tmvMemProbe.c
	$(CC) $(CFLAGS) tmvMemProbe.c $(LIBS) -o tmvMemProbe.elf
	vita-elf-create -e tmvMemProbe.yml tmvMemProbe.elf tmvMemProbe.velf
	vita-make-fself tmvMemProbe.velf tmvMemProbe.skprx
