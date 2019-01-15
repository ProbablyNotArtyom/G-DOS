
#ifdef CONFIG_HAVE_UNDERSCORE_SYMBOL_PREFIX
#define __VMLINUX_SYMBOL(x) _##x
#define __VMLINUX_SYMBOL_STR(x) "_" #x
#else
#define __VMLINUX_SYMBOL(x) x
#define __VMLINUX_SYMBOL_STR(x) #x
#endif

#define TEXT_TEXT\
	__text_start = .;\
	*(.text .text.*)\
	. = ALIGN(0x4);\
  	*(.rodata .rodata.*)\
	__text_end = .;

#define DATA_DATA\
AT ( ADDR(.text) + SIZEOF (.text) ) {\
	. = ALIGN(0x4);\
 	__data_start = . ;\
 	*(.data);\
 	__data_end = . ;

#define BSS_BSS\
	. = ALIGN(0x4);\
	__bss_start = .;\
	*(.bss)\
	_end = .;

#define INIT_CALLS_LEVEL(level)						\
	VMLINUX_SYMBOL(__initcall##level##_start) = .;		\
	*(.initcall##level##.init)				\
	*(.initcall##level##s.init)				\

#define INIT_CALLS							\
	VMLINUX_SYMBOL(__initcall_start) = .;			\
	*(.initcallearly.init)					\
	INIT_CALLS_LEVEL(0)					\
	INIT_CALLS_LEVEL(1)					\
	INIT_CALLS_LEVEL(2)					\
	INIT_CALLS_LEVEL(3)					\
	INIT_CALLS_LEVEL(4)					\
	INIT_CALLS_LEVEL(5)					\
	INIT_CALLS_LEVEL(rootfs)				\
	INIT_CALLS_LEVEL(6)					\
	INIT_CALLS_LEVEL(7)					\
	VMLINUX_SYMBOL(__initcall_end) = .;
