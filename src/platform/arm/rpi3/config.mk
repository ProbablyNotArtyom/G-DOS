
BAREMETAL = TRUE
CPU = cortex-a53
PREFIX = arm-none-eabi-

CC = $(PREFIX)gcc
LD = $(PREFIX)ld
AS = $(PREFIX)as
AR = $(PREFIX)ar
CPY = $(PREFIX)objcopy
RANLIB=$(PREFIX)ranlib

LDFLAGS = -Bstatic
CCFLAGS = -fno-builtin -ffreestanding -nostdlib -static -mcpu=$(CPU)
CCFLAGS_GENERIC = -fno-builtin -ffreestanding -nostdlib  -static -mcpu=$(CPU)
LDLIBS := `$(CC) -print-libgcc-file-name`
