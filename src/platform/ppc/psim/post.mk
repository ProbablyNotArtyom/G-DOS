.PHONY: post run run-debug

post:
	@cd ./bin && $(CPY) -O binary $(BINARY_NAME) $(BINARY_NAME).bin

run:
	cd $(BASEDIR)/src/platform/$(ARCH)/$(PLATFORM) && xterm -bg black -selbg white -e "stty raw -echo onlcr -icanon -iexten onocr opost ofdel; $(PREFIX)run -r 128m -m $(CPU) -e  netbsd -f psim.init $(BINARY_NAME); sleep 10"

run-debug:
	cd $(BASEDIR)/src/platform/$(ARCH)/$(PLATFORM) && $(PREFIX)gdb --command=.gdbinit
