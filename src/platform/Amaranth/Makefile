SOURCES := $(shell find `pwd` -maxdepth 1 -iname '*.c')
SOURCES += $(shell find `pwd` -maxdepth 1 -iname '*.S')
OBJECTS := $(SOURCES:%.c=%.o)
OBJECTS := $(OBJECTS:%.S=%.o)
ARCHNAME := plat-extras

all: clean
	make objects
	$(AR) Prvs $(ARCHNAME).a $(OBJECTS)
	cp $(ARCHNAME).a $(BINDIR)

clean:
	rm -vf *.a
	rm -vf *.o

objects:
	for file in $(SOURCES); do \
		$(CC) $(CCFLAGS) -c $$file; \
	done
