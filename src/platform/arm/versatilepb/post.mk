.PHONY: post
.PHONY: run

.PHONY: rescue
post:
	@cd ./bin && $(CPY) -O binary $(BINARY_NAME) $(BINARY_NAME).bin

run:
	@qemu-system-arm -M versatilepb -m 128M -kernel $(BINARY_NAME) -nographic

run-debug:
	@qemu-system-arm -M versatilepb -m 128M -kernel $(BINARY_NAME)

$(BINDIR)/romdisk.o: $(USRLIBC)
	@dd if=/dev/zero of=$(BINDIR)/romdisk.img bs=1024 count=1024 status=none
	@echo "[DEP] making root filesystem"
	@echo `mkfs.fat $(BINDIR)/romdisk.img -F 12 -s1 -f1` > /dev/null
	@mkdir -p $(BINDIR)/tmproot
	@sudo mount -o loop $(BINDIR)/romdisk.img $(BINDIR)/tmproot
	@sudo cp -r ./root/* $(BINDIR)/tmproot
	@sudo umount $(BINDIR)/tmproot
	@sudo rm -r $(BINDIR)/tmproot
	@cd $(BINDIR) && $(CPY) -I binary -O elf32-littlearm -B $(ARCH) --rename-section .data=.text ./romdisk.img ./romdisk.o
	@rm $(BINDIR)/romdisk.img

.PHONY: rescue
rescue:
	@sudo umount -fq $(BINDIR)/tmproot || /bin/true
	@rm -rf $(BINDIR)/tmproot
