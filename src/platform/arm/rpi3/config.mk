
DONT_BUILD_USR = FALSE
BAREMETAL = TRUE
ROMDISK_SIZE = 2048

CPU = cortex-a7
PREFIX = arm-none-eabi-

CC = $(PREFIX)gcc
LD = $(PREFIX)ld
AS = $(PREFIX)as
AR = $(PREFIX)ar
CPY = $(PREFIX)objcopy
RANLIB=$(PREFIX)ranlib

LDFLAGS = -Bstatic
CCFLAGS = -fno-builtin -ffreestanding -nostdlib -static -mcpu=$(CPU) -marm -mbe32 -mabi=aapcs-linux
LDLIBS := `$(CC) -print-libgcc-file-name`
