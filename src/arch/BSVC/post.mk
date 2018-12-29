.PHONY: post
post: 
	cd ./bin && $(CPY) -I elf32-m68k -O srec Impact Impact.s68 
	cd ./bin && $(CPY) -I elf32-m68k -O binary Impact Impact.bin 

.PHONY: run
run: all
	bsvc ./src/arch/BSVC/BSVC.setup ./bin/Impact.s68