.PHONY: post run run-debug
	
post:
	@cd ./bin && $(CPY) -O binary $(BINARY_NAME) kernel7.bin

run:
	@qemu-system-aarch64 -M raspi2 -kernel $(BINARY_NAME) -serial stdio

run-debug:
	konsole -e qemu-system-aarch64 -s -M raspi2 -kernel $(BINARY_NAME) -serial stdio & arm-none-eabi-gdb $(BINARY_NAME) -ex "target remote localhost:1234" -ex "c"
