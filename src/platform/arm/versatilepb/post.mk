.PHONY: post
.PHONY: run

.PHONY: rescue
post:
	@cd ./bin && $(CPY) -O binary $(BINARY_NAME) $(BINARY_NAME).bin

run:
	@qemu-system-arm -M versatilepb -m 128M -kernel $(BINARY_NAME) -nographic -monitor telnet::45454,server,nowait -serial mon:stdio

run-debug:
	konsole -e qemu-system-arm -s -M versatilepb -m 128M -kernel $(BINARY_NAME) -nographic -monitor telnet::45454,server,nowait -serial mon:stdio & arm-none-eabi-gdb $(BINARY_NAME) -ex "target remote localhost:1234" -ex "c"

$(BINDIR)/romdisk.o: $(USRLIBC)
	@dd if=/dev/zero of=$(BINDIR)/romdisk.img bs=1024 count=1024 status=none
	@echo "[DEP] making root filesystem"
	@echo `mkfs.fat $(BINDIR)/romdisk.img -F 12 -s1 -f1` > /dev/null
	@mkdir -p $(BINDIR)/tmproot
	@$(SUDO) mount -o loop $(BINDIR)/romdisk.img $(BINDIR)/tmproot
	@$(SUDO) cp -r ./root/* $(BINDIR)/tmproot
	@$(SUDO) umount $(BINDIR)/tmproot
	@$(SUDO) rm -r $(BINDIR)/tmproot
	@cd $(BINDIR) && $(CPY) -I binary -O elf32-littlearm -B $(ARCH) --rename-section .data=.text ./romdisk.img ./romdisk.o
	@rm $(BINDIR)/romdisk.img

.PHONY: rescue
rescue:
	@$(SUDO) umount -fq $(BINDIR)/tmproot || /bin/true
	@rm -rf $(BINDIR)/tmproot

.PHONY: dump
dump: $(BINARY_NAME)
	$(PREFIX)objdump -D $(BINARY_NAME) | less
