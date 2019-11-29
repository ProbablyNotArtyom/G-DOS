.PHONY: post
post:
	@cd $(BINDIR) && $(CPY) -I elf32-m68k -O srec $(BINARY_NAME) $(BINARY_NAME).s68
	@cd $(BINDIR) && $(CPY) -I elf32-m68k -O binary $(BINARY_NAME) $(BINARY_NAME).bin

.PHONY: run
run:
	@emu68k $(BINARY_NAME).bin

$(BINDIR)/romdisk.o: $(USRLIBC) $(PLATFORM_CONFIG)
	@dd if=/dev/zero of=$(BINDIR)/romdisk.img bs=1024 count=512 status=none
	@echo "[DEP] making root filesystem"
	@echo `mkfs.fat $(BINDIR)/romdisk.img -F 12 -s1 -f1` > /dev/null
	@mkdir -p $(BINDIR)/tmproot
	@sudo mount -o loop $(BINDIR)/romdisk.img $(BINDIR)/tmproot
	@sudo cp -r ./root/* $(BINDIR)/tmproot
	@sudo umount $(BINDIR)/tmproot
	@sudo rm -r $(BINDIR)/tmproot
	@cd $(BINDIR) && $(CPY) -I binary -O elf32-m68k -B $(ARCH) --rename-section .data=.text ./romdisk.img ./romdisk.o
	@rm $(BINDIR)/romdisk.img

.PHONY: rescue
rescue:
	@sudo umount -fq $(BINDIR)/tmproot || /bin/true
	@rm -rf $(BINDIR)/tmproot
