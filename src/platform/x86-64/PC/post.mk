.PHONY: post
.PHONY: run
.PHONY: rootfs
post:
	cd ./bin && $(CPY) -O binary Impact Impact.bin

run:
	qemu-system-arm -M versatilepb -m 128M -kernel $(BINDIR)/Impact -nographic

rootfs:
	dd if=/dev/zero of=./bin/GDOS.img bs=1024 count=1440
	mkfs.fat ./bin/GDOS.img
	mkdir -p ./bin/tmproot
	sudo mount -o loop ./bin/GDOS.img ./bin/tmproot
	sudo cp -r ./root/* ./bin/tmproot
	sudo cp ./bin/Impact ./bin/tmproot
	sudo umount ./bin/tmproot
	sudo rm -r ./bin/tmproot
