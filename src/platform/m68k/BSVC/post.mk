.PHONY: post
post:
	cd ./bin && $(CPY) -I elf32-m68k -O srec Impact Impact.s68
	cd ./bin && $(CPY) -I elf32-m68k -O binary Impact Impact.bin

.PHONY: run
run:
	bsvc ./src/platform/$(ARCH)/$(PLATFORM)/BSVC.setup ./bin/Impact.s68

.PHONY: rootfs
rootfs:
	dd if=/dev/zero of=$(BINDIR)/fatdisk.img bs=1024 count=256
	mkfs.fat $(BINDIR)/fatdisk.img -F 12 -s1 -f1
	mkdir -p $(BINDIR)/tmproot
	sudo mount -o loop $(BINDIR)/fatdisk.img $(BINDIR)/tmproot
	sudo cp -r ./root/* $(BINDIR)/tmproot
	sudo umount $(BINDIR)/tmproot
	sudo rm -r $(BINDIR)/tmproot
	cd $(BINDIR) && $(CPY) -I binary -O elf32-m68k -B $(ARCH) --rename-section .data=.text ./fatdisk.img ./fatdisk.o
	rm $(BINDIR)/fatdisk.img

.PHONY: rescue
rescue:
	sudo umount -fq $(BINDIR)/tmproot || /bin/true
	rm -rf $(BINDIR)/tmproot
