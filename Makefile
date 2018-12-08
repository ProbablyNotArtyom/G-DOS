export

include ${PWD}/src/arch/${PLATFORM}/config.mk
BINDIR := $(PWD)/bin

SUBDIRS = src

all: clean
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done
	make romfs
	make link
	make post

clean:
	mkdir -p $(BINDIR)
	rm $(BINDIR)/* -vf
	rm ./src/*.o $(OBJS) -vrf
	rm ./src/*.S -vrf
	find ./ -maxdepth 10 -type l -delete
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done

romfs:
	rm -f ./bin/ROM0.o
	./tool/genromfs/genromfs -f ./bin/romfs.dat -d ./root/ -V ROM0
	$(PREFIX)objcopy -I binary -O elf32-m68k -B m68k ./bin/romfs.dat ./bin/ROM0.o

link:
	cd ./bin && for lib in `find . -name '*.a'` ; do \
		$(AR) -x $$lib; \
	done
	cd ./bin && $(LD)  *.o $(LDFLAGS) $(LDLIBS) -o Impact

-include ${PWD}/src/arch/${PLATFORM}/post.mk