export

include ${PWD}/src/platform/${PLATFORM}/config.mk
BINDIR := $(PWD)/bin
CCFLAGS := $(CCFLAGS) -include $(PWD)/src/platform/${PLATFORM}/hwdeps.h -I $(PWD)/src/cpu/$(ARCH)/include -I ${PWD}/src/include -O3
SUBDIRS = src
LDFLAGS := -T $(PWD)/src/platform/${PLATFORM}/link.ld
all: clean
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done
	make link
	make post

clean:
	mkdir -p $(BINDIR)
	rm $(BINDIR)/* -vf
	rm ./src/*.o $(OBJS) -vrf
	find ./ -maxdepth 10 -type l -delete
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done
	$(MAKE) -C usr/lib/ -f Makefile clean

link:
	cd ./bin && $(LD) --whole-archive *.a --no-whole-archive $(LDLIBS) $(LDFLAGS) -o Impact

lib:
	cp -f src/platform/$(PLATFORM)/config.mk usr/config.mk
	cp -f src/platform/$(PLATFORM)/loaded-link.ld usr/link.ld
	cp -f src/cpu/$(ARCH)/export/loaded-startup.S usr/lib/crt0.S
	cp -f `find src/fs/ -maxdepth 10 -name "*.h" -print0 -printf ' '` usr/lib/fs/
	cp -rf src/include/ usr/lib/
	cp -f `find src/lib/ -maxdepth 10 -name "*.c" -print0 -printf ' '` usr/lib/
	cp -rf src/cpu/$(ARCH)/include/* usr/lib/include/
	$(MAKE) -C usr/lib/ -f Makefile

-include ${PWD}/src/platform/${PLATFORM}/post.mk
