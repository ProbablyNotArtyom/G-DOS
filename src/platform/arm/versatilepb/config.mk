
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
CCFLAGS = -fno-builtin -fno-exceptions -fno-unwind-tables -w -mfloat-abi=soft -nostdlib -static -mcpu=$(CPU) -marm -mbe32 -mabi=aapcs-linux # apcs-gnu, atpcs, aapcs, aapcs-linux and iwmmxt.
CCFLAGS_GENERIC = -fno-builtin -fno-exceptions -fno-unwind-tables -w -mfloat-abi=soft -nostdlib -static -mcpu=$(CPU) -marm -mbe32 -mabi=aapcs-linux
LDLIBS := `$(CC) -print-libgcc-file-name`
