.PHONY: post
post:
	@cd $(BINDIR) && $(CPY) -I elf32-m68k -O srec $(BINARY_NAME) $(BINARY_NAME).s68
	@cd $(BINDIR) && $(CPY) -I elf32-m68k -O binary $(BINARY_NAME) $(BINARY_NAME).bin
