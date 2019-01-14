SOURCES := $(shell find . -maxdepth 1 -name '*.c')
SOURCES += $(shell find . -maxdepth 1 -name '*.S')
OBJECTS := $(SOURCES:%.c=%.o)
OBJECTS := $(OBJECTS:%.S=%.o)
SUBDIRS = $(ARCH)

all: clean
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done

clean:
	rm -vf *.a
	rm -vf *.o
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done
