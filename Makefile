#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# G-DOS MASTER MAKEFILE
#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#
# This makefile contains nearly the entire build system for G-DOS.
# It automatically finds all sources and handles them appropriately.
# The only other makefiles in the entire project are located in the
# platform target directory. The platform target must implement:
#    "config.mk"   | platform specific variable definitions
#    "post.mk"     | platform specific make targets
#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

# Read in the build system config file
export
ifneq ("$(wildcard $(PWD)/.config)","")
include $(PWD)/.config
include $(PWD)/src/platform/$(ARCH)/$(PLATFORM)/config.mk
include $(PWD)/src/cpu/$(ARCH)/arch.mk
endif

# Error out if the required fields arent specified
ifneq ("$(MAKECMDGOALS)","clean")
ifndef ARCH
$(error "[!] ARCH is not set. Either pass it as an environment variable or use the config script.")
endif
ifndef PLATFORM
$(error "[!] PLATFORM is not set. Either pass it as an environment variable or use the config script.")
endif
endif

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Define directories to use

BASEDIR := $(PWD)
ROOTDIR := $(BASEDIR)/root
BINDIR := $(BASEDIR)/bin
LIBDIR := $(BASEDIR)/lib
USRBINDIR := $(BASEDIR)/usr

USRBINDIR := $(ROOTDIR)/bin
USRLIBC := $(LIBDIR)/libc.a

BINARY_NAME := $(BINDIR)/gdos

# If doas is present on the host, use it instead of sudo
# This allows you to bypass the annoying timeout when building ROMdisks
ifneq ("$(shell which doas)","")
SUDO := $(shell which doas)
else
SUDO := $(shell which sudo)
endif

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Append the default flags to the ones supplied by the target

CCFLAGS := $(CCFLAGS) -include $(BASEDIR)/src/platform/$(ARCH)/$(PLATFORM)/hwdeps.h -I $(BASEDIR)/src/platform/$(ARCH)/$(PLATFORM) -I $(BASEDIR)/src/cpu/$(ARCH)/include -I $(BASEDIR)/src/include
CCFLAGS_USR := $(CCFLAGS) -O3
LDFLAGS := -T $(BASEDIR)/src/platform/$(ARCH)/$(PLATFORM)/link.ld $(LDFLAGS)
LDFLAGS_USR := $(-marm -mbe32 -mabi=aapcs-linux)

SUBDIRS = src

# Append the debug flag to the CFLAGS if debug was passed in the config
ifeq ("$(DEBUG)","true")
CCFLAGS := $(CCFLAGS) -g
endif

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Find sources & generate object targets

# Search source directories for any sources to build
SOURCES := $(filter-out $(BASEDIR)/src/cpu/% $(BASEDIR)/src/platform/%, $(shell find $(BASEDIR)/src -name '*.c')) $(shell find $(BASEDIR)/src/cpu/$(ARCH) -maxdepth 1 -name '*.c') $(shell find $(BASEDIR)/src/platform/$(ARCH)/$(PLATFORM) -maxdepth 1 -name '*.c')
ARCHLIBSOURCES := $(shell find $(BASEDIR)/src/cpu/$(ARCH)/libkern/ -name '*.c')
USRLIBSOURCES := $(shell find $(BASEDIR)/src/cpu/$(ARCH)/lib/ -name '*.c')
SOURCES_ASM := $(filter-out $(BASEDIR)/src/cpu/% $(BASEDIR)/src/platform/%, $(shell find $(BASEDIR)/src -name '*.S')) $(shell find $(BASEDIR)/src/cpu/$(ARCH) -maxdepth 1 -name '*.S') $(shell find $(BASEDIR)/src/platform/$(ARCH)/$(PLATFORM) -maxdepth 1 -name '*.S')
ARCHLIBSOURCES_ASM := $(shell find $(BASEDIR)/src/cpu/$(ARCH)/libkern/ -name '*.S')
USRLIBSOURCES_ASM := $(shell find $(BASEDIR)/src/cpu/$(ARCH)/lib/ -name '*.S')
USRSOURCES := $(shell find $(BASEDIR)/usr -maxdepth 1 -iname '*.c')

# Create object lists from each source list
OBJECTS_ASM := $(patsubst $(BASEDIR)/%.S, $(BINDIR)/%.o, $(SOURCES_ASM))
ARCHLIBOBJECTS_ASM := $(foreach tmp, $(ARCHLIBSOURCES_ASM:%.S=%.o), $(BINDIR)/src/lib/$(notdir $(tmp)))
USRLIBOBJECTS_ASM := $(foreach tmp, $(USRLIBSOURCES_ASM:%.S=%.o), $(LIBDIR)/bin/$(notdir $(tmp)))
OBJECTS := $(patsubst $(BASEDIR)/%.c, $(BINDIR)/%.o, $(SOURCES))
ARCHLIBOBJECTS := $(foreach tmp, $(ARCHLIBSOURCES:%.c=%.o), $(BINDIR)/src/lib/$(notdir $(tmp)))
USRLIBOBJECTS := $(foreach tmp, $(USRLIBSOURCES:%.c=%.o), $(LIBDIR)/bin/$(notdir $(tmp)))
USROBJECTS := $(USRSOURCES:%.c=%)

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Build rules

.PHONY: all
all:
	@echo "[DEP] Setting up directories"
	@mkdir -p $(LIBDIR)
	@cp $(PWD)/src/platform/$(ARCH)/$(PLATFORM)/config.mk $(LIBDIR)/
	@cp $(PWD)/src/include $(LIBDIR) -rf
	@mkdir -p $(LIBDIR)/include/cpu
	@cp $(PWD)/src/cpu/$(ARCH)/include/* $(LIBDIR)/include/cpu -rf
	@cp $(PWD)/src/cpu/$(ARCH)/lib/link.ld $(LIBDIR)
	mkdir -p $(BINDIR)
	@echo "[CCFLAGS] = $(CCFLAGS)"
	@$(MAKE) deps

.PHONY: deps
deps: $(OBJECTS) $(OBJECTS_ASM) $(ARCHLIBOBJECTS) $(ARCHLIBOBJECTS_ASM) $(USRLIBOBJECTS) $(USRLIBOBJECTS_ASM)
	@$(MAKE) $(USRLIBC)
	@mkdir -p $(ROOTDIR)/bin $(ROOTDIR)/etc $(ROOTDIR)/home $(ROOTDIR)/usr
ifneq ("$(DONT_BUILD_USR)","TRUE")
	@$(MAKE) $(USROBJECTS)
endif
	@$(MAKE) $(BINDIR)/romdisk.o || $(MAKE) rescue
	@$(MAKE) $(BINARY_NAME)
	@$(MAKE) post

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Recursive build targets for all kernel sources

.SECONDEXPANSION :
$(BINARY_NAME): $(OBJECTS) $(OBJECTS_ASM) $(ARCHLIBOBJECTS) $(ARCHLIBOBJECTS_ASM) $(USRLIBOBJECTS) $(USRLIBOBJECTS_ASM) usr
	@echo "[LD] Creating final binary"
	@$(LD) $(shell find $(BINDIR) -name '*.o') $(LDLIBS) $(LDFLAGS) -o $@

$(OBJECTS): $$(patsubst $$(BINDIR)%.o, $$(BASEDIR)%.c, $$@)
	@echo "[CC] -c $(shell realpath -m --relative-to=$(PWD) $(patsubst $(BINDIR)%, $(BASEDIR)%, $(@:%.o=%.c))) -o $(shell realpath -m --relative-to=$(PWD) $(@))"
	@mkdir -p $(dir $@)
	@$(CC) $(CCFLAGS) -c $(patsubst $(BINDIR)%, $(BASEDIR)%, $(@:%.o=%.c)) -o $@

$(ARCHLIBOBJECTS): $$(patsubst $$(BINDIR)/src/lib/%.o, $$(BASEDIR)/src/cpu/$(ARCH)/libkern/%.c, $$@)
	@echo "[CC] -c $(shell realpath -m --relative-to=$(PWD) $(patsubst $(BINDIR)/src/lib/%, $(BASEDIR)/src/cpu/$(ARCH)/libkern/%, $(@:%.o=%.c))) -o $(shell realpath -m --relative-to=$(PWD) $(@))"
	@$(CC) $(CCFLAGS) -c $(patsubst $(BINDIR)/src/lib/%, $(BASEDIR)/src/cpu/$(ARCH)/libkern/%, $(@:%.o=%.c)) -o $@

$(OBJECTS_ASM): $$(patsubst $$(BINDIR)%.o, $$(BASEDIR)%.S, $$@)
	@echo "[CC] -c $(shell realpath -m --relative-to=$(PWD) $(patsubst $(BINDIR)%, $(BASEDIR)%, $(@:%.o=%.S))) -o $(shell realpath -m --relative-to=$(PWD) $(@))"
	@mkdir -p $(dir $@)
	@$(CC) $(CCFLAGS) -c $(patsubst $(BINDIR)%, $(BASEDIR)%, $(@:%.o=%.S)) -o $@

$(ARCHLIBOBJECTS_ASM): $$(patsubst $$(BINDIR)/src/lib/%.o, $$(BASEDIR)/src/cpu/$(ARCH)/libkern/%.S, $$@)
	@echo "[CC] -D _DONT_ADD_STD_STREAMS=true -c $(shell realpath -m --relative-to=$(PWD) $(patsubst $(BINDIR)/src/lib/%, $(BASEDIR)/src/cpu/$(ARCH)/libkern/%, $(@:%.o=%.S))) -o $(shell realpath -m --relative-to=$(PWD) $(@))"
	@$(CC) $(CCFLAGS) -D _DONT_ADD_STD_STREAMS=true -c $(patsubst $(BINDIR)/src/lib/%, $(BASEDIR)/src/cpu/$(ARCH)/libkern/%, $(@:%.o=%.S)) -o $@

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# ROMdisk build targets

# Builds the final ROMdisk object
$(BINDIR)/romdisk.o: $(USRLIBC)
	@dd if=/dev/zero of=$(BINDIR)/romdisk.img bs=1024 count=$(ROMDISK_SIZE) status=none
	@echo "[DEP] making root filesystem"
ifeq ($(shell expr $(ROMDISK_SIZE) \> 2048), 1)
	echo `mkfs.fat $(BINDIR)/romdisk.img -F 16 -s1 -f1` > /dev/null
else
	echo `mkfs.fat $(BINDIR)/romdisk.img -F 12 -s1 -f1` > /dev/null
endif
	@mkdir -p $(BINDIR)/tmproot
	@$(SUDO) mount -o loop $(BINDIR)/romdisk.img $(BINDIR)/tmproot
	@$(SUDO) cp -r ./root/* $(BINDIR)/tmproot
	@$(SUDO) umount $(BINDIR)/tmproot
	@$(SUDO) rm -r $(BINDIR)/tmproot
	@cp $(BASEDIR)/src/platform/romdisk_wrapper.S $(BINDIR)
	@$(CC) -Wa,-I$(BINDIR) $(CCFLAGS) -c $(BINDIR)/romdisk_wrapper.S -o $(BINDIR)/romdisk.o
	#@rm $(BINDIR)/romdisk.img

# Builds and installs the userspace programs to the ROMdisk root
$(USROBJECTS): $(USRLIBC)
	@echo "[USR][CC] -c $(shell echo $@ | rev | cut -f -1 -d '/' | rev ).c -o $(shell echo $@ | rev | cut -f -1 -d '/' | rev).o"
	@$(CC) $(CCFLAGS_USR) -I$(LIBDIR)/include -c $@.c -o $@.o
	@echo "[USR][LD] $(shell echo $@ | rev | cut -f -1 -d '/' | rev).o -o $(shell echo $@ | rev | cut -f -1 -d '/' | rev )"
	@$(LD) $@.o $(LDFLAGS_USR) -L$(LIBDIR) -z nodefaultlib -lc -T $(LIBDIR)/link.ld $(LDLIBS) -o $@
	@cp $(shell echo $(@:%.o=%.c) | cut -f 1 -d '.') $(USRBINDIR)

# Builds the userspace C library
$(USRLIBC): $(USRLIBOBJECTS) $(USRLIBOBJECTS_ASM)  $(shell find $(BINDIR)/src/lib -iname '*.o')
	@$(AR) -rcs $@ $^
	@echo "[AR] archiving: $^"
	@echo "[AR] created: $@"

$(USRLIBOBJECTS): $$(patsubst $$(LIBDIR)/bin/%.o, $$(BASEDIR)/src/cpu/$(ARCH)/lib/%.c, $$@)
	@echo "[CC] -D _DONT_ADD_STD_STREAMS=true -c $(shell realpath -m --relative-to=$(PWD) $(patsubst $(LIBDIR)/bin/%, $(BASEDIR)/src/cpu/$(ARCH)/lib/%, $(@:%.o=%.c))) -o $(shell realpath -m --relative-to=$(PWD) $(@))"
	@mkdir -p $(dir $@)
	@$(CC) -I$(LIBDIR)/include -c $(patsubst $(LIBDIR)/bin/%, $(BASEDIR)/src/cpu/$(ARCH)/lib/%, $(@:%.o=%.c)) -D _DONT_ADD_STD_STREAMS=true $(CCFLAGS_USR) -o $@

$(USRLIBOBJECTS_ASM): $$(patsubst $$(LIBDIR)/bin/%.o, $$(BASEDIR)/src/cpu/$(ARCH)/lib/%.S, $$@)
	@echo "[CC] -c $(shell realpath -m --relative-to=$(PWD) $(patsubst $(LIBDIR)/bin/%, $(BASEDIR)/src/cpu/$(ARCH)/lib/%, $(@:%.o=%.S))) -o $(shell realpath -m --relative-to=$(PWD) $(@))"
	@mkdir -p $(dir $@)
	@$(CC) -I$(LIBDIR)/include -c $(patsubst $(LIBDIR)/bin/%, $(BASEDIR)/src/cpu/$(ARCH)/lib/%, $(@:%.o=%.S)) $(CCFLAGS_USR) -o $@

# This target removes any mounts in the event that generation of the romdisk fails
.PHONY: rescue
rescue:
	@$(SUDO) umount -fq $(BINDIR)/tmproot || /bin/true
	@rm -rf $(BINDIR)/tmproot
	@$(error "[USR][!] Error generating root filesystem image! rescuing...")

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Generic static targets

.PHONY: clean
clean:
	rm -rf $(BINDIR)/* -vf
	rm -rf $(LIBDIR)/* -vrf
	rm -rf $(USRBINDIR)/*
	rm -rf $(USROBJECTS)
	rm -rf $(patsubst %, %.o, $(USROBJECTS))
	rm -rf $(ROOTDIR)/usr/*
	rm -rf $(ROOTDIR)/bin/*

.PHONY: distclean
distclean: clean
	rm -vf $(PWD)/.config

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Append the platform target specific targets

-include $(BASEDIR)/src/platform/$(ARCH)/$(PLATFORM)/post.mk
