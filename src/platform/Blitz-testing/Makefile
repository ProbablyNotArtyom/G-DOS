
SOURCES := $(shell find . -maxdepth 1 -name '*.c')
SOURCES += $(shell find . -maxdepth 1 -name '*.S')
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
