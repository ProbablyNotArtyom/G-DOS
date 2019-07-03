SOURCES := $(shell find `pwd` -maxdepth 1 -iname '*.c')
SOURCES += $(shell find `pwd` -maxdepth 1 -iname '*.S')
OBJECTS := $(SOURCES:%.c=%.o)
OBJECTS := $(OBJECTS:%.S=%.o)
ARCHNAME := cpu-lib
SUBDIRS =


.PHONY: clean all
all: clean
	make objects
	$(AR) rcs libc.a $(OBJECTS)
	cp libc.a $(LIBDIR)
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done

	cp link.ld $(LIBDIR)
	mkdir -p $(LIBDIR)/fs
	cp ../../../fs/fat.h $(LIBDIR)/fs
	cp ../../../fs/fatconf.h $(LIBDIR)/fs

clean:
	rm -vf *.a
	rm -vf *.o
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done

objects:
	for file in $(SOURCES); do \
		$(CC) -I$(LIBDIR)/include -c $$file $(CCFLAGS_GENERIC); \
	done
