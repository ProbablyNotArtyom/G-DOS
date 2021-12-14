
DONT_BUILD_USR = FALSE
BAREMETAL = TRUE
ROMDISK_SIZE = 2048

CPU = arm926ej-s
PREFIX = arm-none-eabi-

CC = $(PREFIX)gcc
LD = $(PREFIX)ld
AS = $(PREFIX)as
AR = $(PREFIX)ar
CPY = $(PREFIX)objcopy
RANLIB=$(PREFIX)ranlib

LDFLAGS = -Bstatic
CCFLAGS = -fno-builtin -fno-exceptions -fno-unwind-tables -fshort-enums -w -mfloat-abi=soft -nostdlib -nostartfiles -static -mcpu=$(CPU) -marm -mbe32 -mabi=aapcs-linux
LDLIBS := `$(CC) -print-libgcc-file-name`
