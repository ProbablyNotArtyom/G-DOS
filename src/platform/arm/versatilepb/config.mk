
BAREMETAL = TRUE
CPU = arm926ej-s
PREFIX = arm-none-eabi-

CC = $(PREFIX)gcc
LD = $(PREFIX)ld
AS = $(PREFIX)as
AR = $(PREFIX)ar
CPY = $(PREFIX)objcopy
RANLIB=$(PREFIX)ranlib

LDFLAGS = -Bstatic
CCFLAGS = -fno-exceptions -fno-unwind-tables -w -mfloat-abi=soft -nostdlib -static -mcpu=$(CPU)
CCFLAGS_GENERIC = -fno-exceptions -fno-unwind-tables -w -mfloat-abi=soft -nostdlib -static -mcpu=$(CPU)
LDLIBS := `$(CC) -print-libgcc-file-name`
