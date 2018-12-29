
BAREMETAL = TRUE
CPU = 68000
PREFIX = m68k-unknown-elf-

CC = $(PREFIX)gcc
LD = $(PREFIX)ld
AS = $(PREFIX)as
AR = $(PREFIX)ar
CPY = $(PREFIX)objcopy

CCFLAGS = -Wall -mcpu=$(CPU) -msoft-float -nostdlib -static -I ${PWD}/src/include -O3
LDFLAGS = -Bstatic -T $(PWD)/src/arch/BSVC/link.ld
LDLIBS := `$(CC) -print-libgcc-file-name`
