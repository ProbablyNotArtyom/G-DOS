export

ifeq ("$(wildcard $(PWD)/.config)","")
$(error .config does not exist. Run config.sh to configure the platform)
endif
include ${PWD}/.config
include ${PWD}/src/platform/${ARCH}/${PLATFORM}/config.mk
BINDIR := $(PWD)/bin
LIBDIR := $(PWD)/lib
USRDIR := $(PWD)/usr
ROOTDIR := $(PWD)/root
CCFLAGS := $(CCFLAGS) -include $(PWD)/src/platform/${ARCH}/${PLATFORM}/hwdeps.h -I $(PWD)/src/cpu/$(ARCH)/include -I ${PWD}/src/include -O3
CCFLAGS_GENERIC := -I $(PWD)/src/cpu/$(ARCH)/include -I ${PWD}/src/include -O3 -mcpu=$(CPU) -nostdlib -static
SUBDIRS = src
LDFLAGS := -T $(PWD)/src/platform/${ARCH}/${PLATFORM}/link.ld

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

all: clean
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done
	make usr
	make rootfs
	make link
	make post

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

clean:
	mkdir -p $(BINDIR)
	rm $(BINDIR)/* -vf
	rm $(LIBDIR)/* -vrf
	$(MAKE) -C $(USRDIR) -f Makefile clean
	cd $(USRDIR) && $(MAKE) uninstall
	rm ./src/*.o $(OBJS) -vrf
	find ./ -maxdepth 10 -type l -delete
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done

distclean: clean
	rm -vf $(PWD)/.config
	sudo rm -vf /usr/bin/$(ARCH)-gdos*

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

link:
	cd ./bin && $(LD) --whole-archive *.a --no-whole-archive `find \`pwd\` -maxdepth 1 -iname '*.o'` $(LDLIBS) $(LDFLAGS) -o Impact

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

.PHONY: lib
lib:
	cp $(PWD)/src/platform/${ARCH}/$(PLATFORM)/config.mk $(PWD)/lib/
	cp $(PWD)/src/include $(PWD)/lib -rf
	cp $(PWD)/src/cpu/$(ARCH)/include/* $(PWD)/lib/include/cpu -rf
	$(MAKE) -C $(PWD)/src/cpu/$(ARCH)/lib/ -f Makefile

usr: lib
	cd $(USRDIR) && $(MAKE) all
	cd $(USRDIR) && $(MAKE) install

-include ${PWD}/src/platform/${ARCH}/${PLATFORM}/post.mk
