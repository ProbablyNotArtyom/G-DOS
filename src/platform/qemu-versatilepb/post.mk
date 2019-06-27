.PHONY: post
.PHONY: run
post: 
	cd ./bin && $(CPY) -O binary Impact Impact.bin 

run:
	qemu-system-arm -M versatilepb -m 128M -kernel $(BINDIR)/Impact -nographic
