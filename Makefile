export

include ${PWD}/src/platform/${PLATFORM}/config.mk
BINDIR := $(PWD)/bin
CCFLAGS := $(CCFLAGS) -include $(PWD)/src/platform/${PLATFORM}/hwdeps.h -B $(PWD)/src/cpu/$(ARCH)/include
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

link:
	cd ./bin && $(LD) --whole-archive *.a --no-whole-archive $(LDLIBS) $(LDFLAGS) -o Impact

-include ${PWD}/src/platform/${PLATFORM}/post.mk
