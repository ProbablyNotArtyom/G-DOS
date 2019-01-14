
SOURCES := $(shell find . -maxdepth 1 -name '*.c')
SOURCES += $(shell find . -maxdepth 1 -name '*.S')
OBJECTS := $(SOURCES:%.c=%.o)
OBJECTS := $(OBJECTS:%.S=%.o)

SUBDIRS = $(PLATFORM)

all:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done
