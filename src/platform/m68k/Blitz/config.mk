
DONT_BUILD_USR = FALSE
BAREMETAL = TRUE
ROMDISK_SIZE = 384

CPU = 68030
PREFIX = m68k-unknown-elf-

CC = $(PREFIX)gcc
LD = $(PREFIX)ld
AS = $(PREFIX)as
AR = $(PREFIX)ar
CPY = $(PREFIX)objcopy

CCFLAGS = -fno-builtin -mcpu=$(CPU) -msoft-float -nostdlib -static -w
LDFLAGS = -Bstatic
LDLIBS = `$(CC) -print-libgcc-file-name`
