
DONT_BUILD_USR = 1
CPU = 603e
PREFIX = powerpc-linux-linux-gnu-

CC = $(PREFIX)gcc
LD = $(PREFIX)ld
AS = $(PREFIX)as
AR = $(PREFIX)ar
CPY = $(PREFIX)objcopy
RANLIB=$(PREFIX)ranlib

LDFLAGS = -Bstatic -z nodefaultlib --emit-stub-syms
CCFLAGS = -w -nostartfiles -mregnames -fno-stack-protector -m32 -mhard-float -nodefaultlibs -nostdlib -static -mcpu=$(CPU)
CCFLAGS_GENERIC = -w -nostartfiles -mregnames -fno-stack-protector -m32 -mhard-float -nodefaultlibs -nostdlib -static -mcpu=$(CPU)
LDLIBS := `$(CC) -print-libgcc-file-name`
