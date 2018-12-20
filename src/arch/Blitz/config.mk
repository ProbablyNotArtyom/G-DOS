
BAREMETAL = TRUE
CPU = 68030
PREFIX = m68k-linux-gnu-

CC = $(PREFIX)gcc
LD = $(PREFIX)ld
AS = $(PREFIX)as
AR = $(PREFIX)ar
CPY = $(PREFIX)objcopy

CCFLAGS = -Wall -m$(CPU) -nostdlib -static -I ${PWD}/src/include -O3
LDFLAGS = -Bstatic
LDLIBS := -T $(PWD)/src/arch/Blitz/link.ld
