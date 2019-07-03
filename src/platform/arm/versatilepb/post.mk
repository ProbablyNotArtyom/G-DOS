.PHONY: post
.PHONY: run
.PHONY: rootfs
.PHONY: rescue
post:
	cd ./bin && $(CPY) -O binary Impact Impact.bin

run:
	qemu-system-arm -M versatilepb -m 128M -kernel $(BINDIR)/Impact -nographic

rootfs:
	dd if=/dev/zero of=$(BINDIR)/fatdisk.img bs=1024 count=512
	mkfs.fat $(BINDIR)/fatdisk.img -F 12 -s1 -f1
	mkdir -p $(BINDIR)/tmproot
	sudo mount -o loop $(BINDIR)/fatdisk.img $(BINDIR)/tmproot
	sudo cp -r ./root/* $(BINDIR)/tmproot
	sudo umount $(BINDIR)/tmproot
	sudo rm -r $(BINDIR)/tmproot
	cd $(BINDIR) && $(CPY) -I binary -O elf32-littlearm -B $(ARCH) --rename-section .data=.text ./fatdisk.img ./fatdisk.o
	rm $(BINDIR)/fatdisk.img

rescue:
	sudo umount -fq $(BINDIR)/tmproot || /bin/true
	rm -rf $(BINDIR)/tmproot
