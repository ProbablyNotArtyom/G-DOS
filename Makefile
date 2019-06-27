export

ifeq ("$(wildcard $(PWD)/.config)","")
$(error .config does not exist. Run config.sh to configure the platform)
endif
include ${PWD}/.config
include ${PWD}/src/platform/${PLATFORM}/config.mk
BINDIR := $(PWD)/bin
LIBDIR := $(PWD)/lib
USRDIR := $(PWD)/usr
CCFLAGS := $(CCFLAGS) -include $(PWD)/src/platform/${PLATFORM}/hwdeps.h -I $(PWD)/src/cpu/$(ARCH)/include -I ${PWD}/src/include -O3
CCFLAGS_GENERIC := -I $(PWD)/src/cpu/$(ARCH)/include -I ${PWD}/src/include -O3
SUBDIRS = src
LDFLAGS := -T $(PWD)/src/platform/${PLATFORM}/link.ld
all: 
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done
	make link
	make post

clean:
	mkdir -p $(BINDIR)
	rm $(BINDIR)/* -vf
	rm $(LIBDIR)/* -vrf
	$(MAKE) -C $(USRDIR) -f Makefile clean
	rm ./src/*.o $(OBJS) -vrf
	find ./ -maxdepth 10 -type l -delete
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done

distclean: clean
	rm -vf $(PWD)/.config

link:
	cd ./bin && $(LD) --whole-archive *.a --no-whole-archive $(LDLIBS) $(LDFLAGS) -o Impact

.PHONY: lib
lib:
	cp $(PWD)/src/platform/$(PLATFORM)/config.mk $(PWD)/lib/
	$(MAKE) -C $(PWD)/src/cpu/$(ARCH)/lib/ -f Makefile
	cp $(PWD)/src/include $(PWD)/lib -rf
	cp $(PWD)/src/cpu/$(ARCH)/include/* $(PWD)/lib/include/cpu -rf

-include ${PWD}/src/platform/${PLATFORM}/post.mk
