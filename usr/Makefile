SOURCES := $(shell find `pwd` -maxdepth 1 -iname '*.c')
SOURCES += $(shell find `pwd` -maxdepth 1 -iname '*.S')
OBJECTS := $(SOURCES:%.c=%.o)
OBJECTS := $(OBJECTS:%.S=%.o)

INSTALL_DIR = $(ROOTDIR)/usr

LIBDIR = $(PWD)/../lib
-include $(LIBDIR)/config.mk

LDFLAGS = -T $(LIBDIR)/link.ld
CCFLAGS := $(CCFLAGS) -I $(LIBDIR)/include -nostdlib $(LIBDIR)/libc.a -Wl,$(LDLIBS)

all: clean
	make objects

clean:
	rm -vf *.a
	rm -vf *.o
	find . -type f \( ! -name "*.*" ! -name "Makefile" \) -delete

objects:
	for file in $(SOURCES); do \
		$(CC) -T $(LIBDIR)/link.ld $$file $(CCFLAGS) -o `echo $$file| cut -f 1 -d '.'`; \
	done

install:
	for file in $(SOURCES); do \
		cp `echo $$file| cut -f 1 -d '.'` $(INSTALL_DIR); \
	done

uninstall:
	for file in $(SOURCES); do \
		rm -vf $(INSTALL_DIR)/`echo $$file | cut -f 1 -d '.' | rev | cut -f -1 -d '/' | rev `; \
	done
