export
#MAKE := $(MAKE) -s
include ${PWD}/.config
include ${PWD}/src/platform/${ARCH}/${PLATFORM}/config.mk
include ${PWD}/src/cpu/${ARCH}/arch.mk
ifeq ("$(wildcard $(PWD)/.config)","")
$(error .config does not exist. Run config.sh to configure the platform)
endif

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

BASEDIR := $(PWD)
ROOTDIR := $(BASEDIR)/root
BINDIR := $(BASEDIR)/bin
LIBDIR := $(BASEDIR)/lib
USRDIR := $(BASEDIR)/usr

USRBINDIR := $(ROOTDIR)/usr
USRLIBC := $(LIBDIR)/libc.a

BINARY_NAME := $(BINDIR)/gdos

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

CCFLAGS := $(CCFLAGS) -include $(BASEDIR)/src/platform/${ARCH}/${PLATFORM}/hwdeps.h -I $(BASEDIR)/src/cpu/$(ARCH)/include -I ${PWD}/src/include -O3
CCFLAGS_GENERIC := $(CCFLAGS_GENERIC) -I $(LIBDIR)/include
SUBDIRS = src
LDFLAGS := -T $(BASEDIR)/src/platform/${ARCH}/${PLATFORM}/link.ld $(LDFLAGS)

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SOURCES := $(filter-out ./src/cpu/% ./src/platform/%, $(shell find ./src -name '*.c')) $(shell find ./src/cpu/$(ARCH) -maxdepth 1 -name '*.c') $(shell find ./src/platform/$(ARCH)/$(PLATFORM) -maxdepth 1 -name '*.c')
ARCHLIBSOURCES := $(shell find ./src/cpu/$(ARCH)/libkern/ -name '*.c')
USRLIBSOURCES := $(shell find ./src/cpu/$(ARCH)/lib/ -name '*.c')
SOURCES_ASM := $(filter-out ./src/cpu/%, $(shell find ./src -name '*.S')) $(shell find ./src/cpu/$(ARCH) -maxdepth 1 -name '*.S')
ARCHLIBSOURCES_ASM := $(shell find ./src/cpu/$(ARCH)/libkern/ -name '*.S')
USRLIBSOURCES_ASM := $(shell find ./src/cpu/$(ARCH)/lib/ -name '*.S')
OBJECTS := $(foreach tmp, $(SOURCES:%.c=%.o), $(BINDIR)/$(tmp))
ARCHLIBOBJECTS := $(foreach tmp, $(ARCHLIBSOURCES:%.c=%.o), $(BINDIR)/src/lib/$(notdir $(tmp)))
USRLIBOBJECTS := $(foreach tmp, $(USRLIBSOURCES:%.c=%.o), $(LIBDIR)/bin/$(notdir $(tmp)))
OBJECTS_ASM := $(foreach tmp, $(SOURCES_ASM:%.S=%.o), $(BINDIR)/$(tmp))
ARCHLIBOBJECTS_ASM := $(foreach tmp, $(ARCHLIBSOURCES_ASM:%.S=%.o), $(BINDIR)/src/lib/$(notdir $(tmp)))
USRLIBOBJECTS_ASM := $(foreach tmp, $(USRLIBSOURCES_ASM:%.S=%.o), $(LIBDIR)/bin/$(notdir $(tmp)))

USRSOURCES := $(shell find ./usr -maxdepth 1 -iname '*.c')
USROBJECTS := $(USRSOURCES:%.c=%)

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

.PHONY: all
all:
	@echo "[DEP] Setting up directories"
	@cp $(PWD)/src/platform/${ARCH}/$(PLATFORM)/config.mk $(LIBDIR)/
	@cp $(PWD)/src/include $(LIBDIR) -rf
	@cp $(PWD)/src/cpu/$(ARCH)/include/* $(LIBDIR)/include/cpu -rf
	@cp $(PWD)/src/cpu/$(ARCH)/lib/link.ld $(LIBDIR)
	@$(MAKE) deps

.PHONY: deps
deps: $(OBJECTS) $(OBJECTS_ASM) $(ARCHLIBOBJECTS) $(ARCHLIBOBJECTS_ASM) $(USRLIBOBJECTS) $(USRLIBOBJECTS_ASM)
	@$(MAKE) $(USRLIBC)
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
	@$(CC) $(CCFLAGS_GENERIC) -c $@.c -o $@.o
	@echo "[USR][LD] $(shell echo $@ | rev | cut -f -1 -d '/' | rev).o -o $(shell echo $@ | rev | cut -f -1 -d '/' | rev )"
	@$(LD) $@.o -L$(LIBDIR) -lc -Bstatic -T $(LIBDIR)/link.ld -o $@ $(LDLIBS)
	@cp $(shell echo $(@:%.o=%.c) | cut -f 1 -d '.') $(USRBINDIR)

.SECONDEXPANSION :
$(BINARY_NAME): $(OBJECTS) $(OBJECTS_ASM) $(ARCHLIBOBJECTS) $(ARCHLIBOBJECTS_ASM) $(USRLIBOBJECTS) $(USRLIBOBJECTS_ASM) usr
	@echo "[LD] Creating final binary"
	$(LD) $(shell find $(BINDIR) -name '*.o') $(LDLIBS) $(LDFLAGS) -o $@

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
	@$(CC) -I$(LIBDIR)/include -c $(patsubst $(LIBDIR)/bin/%, $(BASEDIR)/src/cpu/$(ARCH)/lib/%, $(@:%.o=%.c)) $(CCFLAGS) -o $@

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
	@$(CC) -I$(LIBDIR)/include -c $(patsubst $(LIBDIR)/bin/%, $(BASEDIR)/src/cpu/$(ARCH)/lib/%, $(@:%.o=%.S)) $(CCFLAGS) -o $@

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

.PHONY: clean
clean:
	mkdir -p $(BINDIR)
	rm -rf $(BINDIR)/* -vf
	rm -rf $(LIBDIR)/* -vrf
	rm -rf $(USRBINDIR)/*
	rm -rf $(patsubst %, %.o, $(USROBJECTS))
	rm -rf $(USROBJECTS)

.PHONY: distclean
distclean: clean
	rm -vf $(PWD)/.config

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

-include ${PWD}/src/platform/${ARCH}/${PLATFORM}/post.mk
