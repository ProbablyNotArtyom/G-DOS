
BAREMETAL = TRUE
CPU = arm926ej-s
PREFIX = arm-none-eabi-

CC = $(PREFIX)gcc
LD = $(PREFIX)ld
AS = $(PREFIX)as
AR = $(PREFIX)ar
CPY = $(PREFIX)objcopy
RANLIB=$(PREFIX)ranlib

CCFLAGS = -fno-exceptions -fno-unwind-tables -Wall -mcpu=$(CPU) -nostdlib -static -mfloat-abi=soft
LDFLAGS = -Bstatic --exclude-libs,libssp
LDLIBS := `$(CC) -print-libgcc-file-name`
