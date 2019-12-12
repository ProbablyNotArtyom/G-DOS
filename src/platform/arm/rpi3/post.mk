.PHONY: post
.PHONY: run

.PHONY: rescue
post:
	@cd ./bin && $(CPY) -O binary $(BINARY_NAME) kernel7.bin

run:
	@qemu-system-aarch64 -M raspi2 -kernel $(BINARY_NAME) -serial stdio

run-debug:
	konsole -e qemu-system-aarch64 -s -M raspi2 -kernel $(BINARY_NAME) -serial stdio & arm-none-eabi-gdb $(BINARY_NAME) -ex "target remote localhost:1234" -ex "c"

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
