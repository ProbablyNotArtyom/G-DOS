
BAREMETAL = TRUE
CPU = 68030
PREFIX = m68k-elf-

CC = $(PREFIX)gcc
LD = $(PREFIX)ld
AS = $(PREFIX)as
AR = $(PREFIX)ar
CPY = $(PREFIX)objcopy

CCFLAGS = -Wall -mcpu=$(CPU) -nostdlib -static -I ${PWD}/src/include -O3
LDFLAGS = -Bstatic 
LDLIBS := `$(CC) -print-libgcc-file-name`
