
BAREMETAL = TRUE
CPU = 68000
PREFIX = m68k-linux-gnu-

CC = $(PREFIX)gcc
LD = $(PREFIX)ld
AS = $(PREFIX)as
AR = $(PREFIX)ar
CPY = $(PREFIX)objcopy

CCFLAGS = -Wall -m$(CPU) -nostdlib -static -I ${PWD}/src -O3
LDFLAGS = -Bstatic 
LDLIBS := -T $(PWD)/src/arch/BSVC/link.ld 