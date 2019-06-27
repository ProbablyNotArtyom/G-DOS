
BAREMETAL = TRUE
ARCH = ppc
CPU = mcp601
PREFIX = powerpc-linux-gnu-

CC = $(PREFIX)gcc
LD = $(PREFIX)ld
AS = $(PREFIX)as
AR = $(PREFIX)ar
CPY = $(PREFIX)objcopy

CCFLAGS = -Wall -mcpu=$(CPU)  -nostdlib -static
LDFLAGS = -Bstatic
LDLIBS := `$(CC) -print-libgcc-file-name`
