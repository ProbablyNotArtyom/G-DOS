SOURCES := $(shell find `pwd` -maxdepth 1 -iname '*.c')
SOURCES += $(shell find `pwd` -maxdepth 1 -iname '*.S')
OBJECTS := $(SOURCES:%.c=%.o)
OBJECTS := $(OBJECTS:%.S=%.o)
SUBDIRS =

include ${PWD}/../config.mk
BINDIR := ./
CCFLAGS := $(CCFLAGS) -I $(PWD)/../lib/include
LDFLAGS := -T $(PWD)/../link.ld

all: clean
	make objects

clean:
	for file in $(SOURCES); do \
		rm -vf ../`echo \`basename $$file\` | cut -f 1 -d '.'`;\
	done
	rm -vf *.a
	rm -vf *.o
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done

objects:
	for file in $(SOURCES); do \
		$(CC) $(CCFLAGS) -c $$file; \
		$(LD) `echo $$file| cut -f 1 -d '.'`.o $(LDLIBS) $(LDFLAGS) -L $(PWD)/../lib/ -lstd -o `echo $$file| cut -f 1 -d '.'`;\
		mv -f `echo \`basename $$file\` | cut -f 1 -d '.'` ../;\
	done
