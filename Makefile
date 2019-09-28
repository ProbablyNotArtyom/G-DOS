export
#MAKE := $(MAKE) -s
ifneq ("$(wildcard $(PWD)/.config)","")
include ${PWD}/.config
include ${PWD}/src/platform/${ARCH}/${PLATFORM}/config.mk
include ${PWD}/src/cpu/${ARCH}/arch.mk
endif

ifneq ("$(MAKECMDGOALS)","clean")
ifndef ARCH
$(error "[!] ARCH is not set. Either pass it as an environment variable or use the config script.")
endif
ifndef PLATFORM
$(error "[!] PLATFORM is not set. Either pass it as an environment variable or use the config script.")
endif
endif

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

BASEDIR := $(PWD)
ROOTDIR := $(BASEDIR)/root
BINDIR := $(BASEDIR)/bin
LIBDIR := $(BASEDIR)/lib
USRBINDIR := $(BASEDIR)/usr

USRBINDIR := $(ROOTDIR)/bin
USRLIBC := $(LIBDIR)/libc.a

BINARY_NAME := $(BINDIR)/gdos

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

CCFLAGS := $(CCFLAGS) -include $(BASEDIR)/src/platform/${ARCH}/${PLATFORM}/hwdeps.h -I $(BASEDIR)/src/platform/${ARCH}/${PLATFORM} -I $(BASEDIR)/src/cpu/$(ARCH)/include -I ${PWD}/src/include
CCFLAGS_GENERIC := $(CCFLAGS_GENERIC) -I $(LIBDIR)/include
CCFLAGS_USR := $(CCFLAGS) -O3
SUBDIRS = src
LDFLAGS := -T $(BASEDIR)/src/platform/${ARCH}/${PLATFORM}/link.ld $(LDFLAGS)

ifeq ("$(DEBUG)","true")
CCFLAGS := $(CCFLAGS) -g
endif

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SOURCES := $(filter-out $(BASEDIR)/src/cpu/% $(BASEDIR)/src/platform/%, $(shell find $(BASEDIR)/src -name '*.c')) $(shell find $(BASEDIR)/src/cpu/$(ARCH) -maxdepth 1 -name '*.c') $(shell find $(BASEDIR)/src/platform/$(ARCH)/$(PLATFORM) -maxdepth 1 -name '*.c')
ARCHLIBSOURCES := $(shell find $(BASEDIR)/src/cpu/$(ARCH)/libkern/ -name '*.c')
USRLIBSOURCES := $(shell find $(BASEDIR)/src/cpu/$(ARCH)/lib/ -name '*.c')
SOURCES_ASM := $(filter-out $(BASEDIR)/src/cpu/%, $(shell find $(BASEDIR)/src -name '*.S')) $(shell find $(BASEDIR)/src/cpu/$(ARCH) -maxdepth 1 -name '*.S')
ARCHLIBSOURCES_ASM := $(shell find $(BASEDIR)/src/cpu/$(ARCH)/libkern/ -name '*.S')
USRLIBSOURCES_ASM := $(shell find $(BASEDIR)/src/cpu/$(ARCH)/lib/ -name '*.S')
OBJECTS := $(patsubst $(BASEDIR)/%.c, $(BINDIR)/%.o, $(SOURCES))
ARCHLIBOBJECTS := $(foreach tmp, $(ARCHLIBSOURCES:%.c=%.o), $(BINDIR)/src/lib/$(notdir $(tmp)))
USRLIBOBJECTS := $(foreach tmp, $(USRLIBSOURCES:%.c=%.o), $(LIBDIR)/bin/$(notdir $(tmp)))
OBJECTS_ASM := $(patsubst $(BASEDIR)/%.S, $(BINDIR)/%.o, $(SOURCES_ASM))
ARCHLIBOBJECTS_ASM := $(foreach tmp, $(ARCHLIBSOURCES_ASM:%.S=%.o), $(BINDIR)/src/lib/$(notdir $(tmp)))
USRLIBOBJECTS_ASM := $(foreach tmp, $(USRLIBSOURCES_ASM:%.S=%.o), $(LIBDIR)/bin/$(notdir $(tmp)))

USRSOURCES := $(shell find $(BASEDIR)/usr -maxdepth 1 -iname '*.c')
USROBJECTS := $(USRSOURCES:%.c=%)

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

.PHONY: all
all:
	@echo "[DEP] Setting up directories"
	@mkdir -p $(LIBDIR)
	@cp $(PWD)/src/platform/${ARCH}/$(PLATFORM)/config.mk $(LIBDIR)/
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
	@$(MAKE) $(USROBJECTS)
	@$(MAKE) $(BINDIR)/romdisk.o
	@$(MAKE) $(BINARY_NAME)
	@$(MAKE) post

$(USRLIBC): $(USRLIBOBJECTS) $(USRLIBOBJECTS_ASM)  $(shell find $(BINDIR)/src/lib -iname '*.o')
	@$(AR) -rcs $@ $^
	@echo "[AR] archiving: $^"
	@echo "[AR] created: $@"

$(USROBJECTS): $(USRLIBC)
	@echo "[USR][CC] -c $(shell echo $@ | rev | cut -f -1 -d '/' | rev ).c -o $(shell echo $@ | rev | cut -f -1 -d '/' | rev).o"
	@$(CC) $(CCFLAGS_USR) -I$(LIBDIR)/include -c $@.c -o $@.o
	@echo "[USR][LD] $(shell echo $@ | rev | cut -f -1 -d '/' | rev).o -o $(shell echo $@ | rev | cut -f -1 -d '/' | rev )"
	@$(LD) $@.o -L$(LIBDIR) -Bstatic -z nodefaultlib -lc -T $(LIBDIR)/link.ld $(LDLIBS) -o $@
	@cp $(shell echo $(@:%.o=%.c) | cut -f 1 -d '.') $(USRBINDIR)

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

$(USRLIBOBJECTS): $$(patsubst $$(LIBDIR)/bin/%.o, $$(BASEDIR)/src/cpu/$(ARCH)/lib/%.c, $$@)
	@echo "[CC] -c $(shell realpath -m --relative-to=$(PWD) $(patsubst $(LIBDIR)/bin/%, $(BASEDIR)/src/cpu/$(ARCH)/lib/%, $(@:%.o=%.c))) -o $(shell realpath -m --relative-to=$(PWD) $(@))"
	@mkdir -p $(dir $@)
	@$(CC) -I$(LIBDIR)/include -c $(patsubst $(LIBDIR)/bin/%, $(BASEDIR)/src/cpu/$(ARCH)/lib/%, $(@:%.o=%.c)) $(CCFLAGS_USR) -o $@

$(OBJECTS_ASM): $$(patsubst $$(BINDIR)%.o, $$(BASEDIR)%.S, $$@)
	@echo "[CC] -c $(shell realpath -m --relative-to=$(PWD) $(patsubst $(BINDIR)%, $(BASEDIR)%, $(@:%.o=%.S))) -o $(shell realpath -m --relative-to=$(PWD) $(@))"
	@mkdir -p $(dir $@)
	@$(CC) $(CCFLAGS) -c $(patsubst $(BINDIR)%, $(BASEDIR)%, $(@:%.o=%.S)) -o $@

$(ARCHLIBOBJECTS_ASM): $$(patsubst $$(BINDIR)/src/lib/%.o, $$(BASEDIR)/src/cpu/$(ARCH)/libkern/%.S, $$@)
	@echo "[CC] -c $(shell realpath -m --relative-to=$(PWD) $(patsubst $(BINDIR)/src/lib/%, $(BASEDIR)/src/cpu/$(ARCH)/libkern/%, $(@:%.o=%.S))) -o $(shell realpath -m --relative-to=$(PWD) $(@))"
	@$(CC) $(CCFLAGS) -c $(patsubst $(BINDIR)/src/lib/%, $(BASEDIR)/src/cpu/$(ARCH)/libkern/%, $(@:%.o=%.S)) -o $@

$(USRLIBOBJECTS_ASM): $$(patsubst $$(LIBDIR)/bin/%.o, $$(BASEDIR)/src/cpu/$(ARCH)/lib/%.S, $$@)
	@echo "[CC] -c $(shell realpath -m --relative-to=$(PWD) $(patsubst $(LIBDIR)/bin/%, $(BASEDIR)/src/cpu/$(ARCH)/lib/%, $(@:%.o=%.S))) -o $(shell realpath -m --relative-to=$(PWD) $(@))"
	@mkdir -p $(dir $@)
	@$(CC) -I$(LIBDIR)/include -c $(patsubst $(LIBDIR)/bin/%, $(BASEDIR)/src/cpu/$(ARCH)/lib/%, $(@:%.o=%.S)) $(CCFLAGS_USR) -o $@

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

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

-include ${PWD}/src/platform/${ARCH}/${PLATFORM}/post.mk
