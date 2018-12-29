
SOURCES := $(shell find . -maxdepth 1 -name '*.c')
SOURCES += $(shell find . -maxdepth 1 -name '*.S')
OBJECTS := $(SOURCES:%.c=%.o)
OBJECTS := $(OBJECTS:%.S=%.o)
ARCHNAME = fs

SUBDIRS =

all:
	make objects
	$(AR) Prvs $(ARCHNAME).a $(OBJECTS)
	cp $(ARCHNAME).a $(BINDIR)
	cp fatROM $(BINDIR)/fatROM.o
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
