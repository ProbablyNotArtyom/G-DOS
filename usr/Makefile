SOURCES := $(shell find `pwd` -maxdepth 1 -iname '*.c')
SOURCES += $(shell find `pwd` -maxdepth 1 -iname '*.S')
OBJECTS := $(SOURCES:%.c=%.o)
OBJECTS := $(OBJECTS:%.S=%.o)
SUBDIRS = utils

include ${PWD}/../config.mk
BINDIR := ./
CCFLAGS := $(CCFLAGS) -I $(PWD)/../lib/include
LDFLAGS := -T $(PWD)/../link.ld

all: clean
	make objects

clean:
	for file in $(SOURCES); do \
		rm -vf ../`echo $$file| cut -f 1 -d '.'`;\
	done
	rm -vf *.a
	rm -vf *.o
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done

objects:
	for file in $(SOURCES); do \
		$(CC) -ffunction-sections -fdata-sections $(CCFLAGS) -c $$file; \
		$(LD) `echo $$file| cut -f 1 -d '.'`.o $(LDLIBS) $(LDFLAGS) -L $(PWD)/../lib/ -lstd -o `echo $$file| cut -f 1 -d '.'` --gc-sections;\
		mv -f `find -maxdepth 1 -not -name "*.c" -and -not -name "*.o" -print0 -printf " "` ../;\
	done
