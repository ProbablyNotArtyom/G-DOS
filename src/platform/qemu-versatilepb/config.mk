
BAREMETAL = TRUE
ARCH = arm
CPU = arm926ej-s
PREFIX = arm-linux-gnueabihf-

CC = $(PREFIX)gcc
LD = $(PREFIX)ld
AS = $(PREFIX)as
AR = $(PREFIX)ar
CPY = $(PREFIX)objcopy

CCFLAGS = -fno-exceptions -fno-unwind-tables -Wall -mcpu=$(CPU) -nostdlib -static
LDFLAGS = -Bstatic --exclude-libs,libssp
LDLIBS := `$(CC) -print-libgcc-file-name`
