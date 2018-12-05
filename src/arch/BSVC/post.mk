.PHONY: post
post: 
	cd ./bin && $(LD)  *.o $(LDFLAGS) $(LDLIBS) -o Impact.out
	cd ./bin && $(CPY) -I elf32-m68k -O srec Impact.out Impact.s68 
	cd ./bin && $(CPY) -I elf32-m68k -O binary Impact.out Impact.bin 

.PHONY: run
run: all
	export ARGS="./src/arch/BSVC/BSVC.setup ./bin/Impact.s68" && exec wish -f ../../Utils/bsvc/src/UI/bsvc.tk