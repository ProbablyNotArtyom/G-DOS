
SOURCES := $(shell find . -maxdepth 1 -name '*.c')
SOURCES += $(shell find . -maxdepth 1 -name '*.S')
OBJECTS := $(SOURCES:%.c=%.o)
OBJECTS := $(OBJECTS:%.S=%.o)
ARCHNAME := cpu
SUBDIRS = lib

all: clean
	make objects
	$(AR) Prvs $(ARCHNAME).a $(OBJECTS)
	cp $(ARCHNAME).a $(BINDIR)
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done

clean:
	rm -vf *.a
	rm -vf *.o
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done

objects:
	for file in $(SOURCES); do \
		$(CC) $(CCFLAGS) -c $$file; \
	done
