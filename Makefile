export

include ${PWD}/src/arch/${PLATFORM}/config.mk
BINDIR := $(PWD)/bin
CCFLAGS := $(CCFLAGS) -include $(PWD)/src/arch/${PLATFORM}/hwdeps.h
SUBDIRS = src

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
	cd ./bin && for lib in `find . -name '*.a'` ; do \
		$(AR) -x $$lib; \
	done
	cd ./bin && $(LD)  *.o $(LDFLAGS) $(LDLIBS) -o Impact

-include ${PWD}/src/arch/${PLATFORM}/post.mk
