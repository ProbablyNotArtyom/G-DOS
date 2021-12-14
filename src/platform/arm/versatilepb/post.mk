.PHONY: post run run-debug

post:
	@cd ./bin && $(CPY) -O binary $(BINARY_NAME) $(BINARY_NAME).bin

run:
	@qemu-system-arm -M versatilepb -m 128M -kernel $(BINARY_NAME) -nographic -monitor telnet::45454,server,nowait -serial mon:stdio

run-debug:
	konsole -e qemu-system-arm -s -M versatilepb -m 128M -kernel $(BINARY_NAME) -nographic -monitor telnet::45454,server,nowait -serial mon:stdio & arm-none-eabi-gdb $(BINARY_NAME) -ex "target remote localhost:1234" -ex "c"
