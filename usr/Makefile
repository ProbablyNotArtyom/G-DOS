SOURCES := $(shell find `pwd` -maxdepth 1 -iname '*.c')
SOURCES += $(shell find `pwd` -maxdepth 1 -iname '*.S')
OBJECTS := $(SOURCES:%.c=%.o)
OBJECTS := $(OBJECTS:%.S=%.o)
SUBDIRS =

LIBDIR = $(PWD)/../lib
-include $(LIBDIR)/config.mk

LDFLAGS = -T $(LIBDIR)/link.ld
CCFLAGS = -I $(LIBDIR)/include -nostdlib

all: clean
	make objects

clean:
	rm -vf *.a
	rm -vf *.o
	find . -type f \( ! -name "*.*" ! -name "Makefile" \) -delete
	
objects:
	for file in $(SOURCES); do \
		$(CC) $(CCFLAGS) -c $$file; \
		$(LD) `echo $$file| cut -f 1 -d '.'`.o $(LDLIBS) $(LDFLAGS) -L $(LIBDIR) -lc -o `echo $$file| cut -f 1 -d '.'`;\
	done
