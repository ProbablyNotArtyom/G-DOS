
BUILD_USR_PROGRAMS := true

BAREMETAL = TRUE
CPU = 68000
PREFIX = m68k-unknown-elf-

CC = $(PREFIX)gcc
LD = $(PREFIX)ld
AS = $(PREFIX)as
AR = $(PREFIX)ar
CPY = $(PREFIX)objcopy

CCFLAGS = -fno-builtin -mcpu=$(CPU) -msoft-float -nostdlib -static -w
CCFLAGS_GENERIC = -fno-builtin -w -msoft-float -nostdlib -static -mcpu=$(CPU)
LDFLAGS = -Bstatic
LDLIBS = `$(CC) -print-libgcc-file-name`
