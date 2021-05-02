
#ifndef _HEADER_INIT
#define _HEADER_INIT

	#include <sys/cdefs.h>
	#include <std.h>
	#include <tui.h>

//---------------------------------------------------

#ifndef __ASSEMBLY__

/*
 * Used for initialization calls..
 */
typedef int (*initcall_t)(void);
extern initcall_t __start_initsec, __stop_initsec;
extern unsigned int reset_devices;


#define __define_initcall(fn, id) \
	static initcall_t __initcall_##fn##id __attribute__((__used__)) \
	__attribute__((__section__(".initcall" #id ".init"))) = fn; \
	LTO_REFERENCE_INITCALL(__initcall_##fn##id)


#define early_initcall(fn)		__define_initcall(fn, early)
#define pure_initcall(fn)		__define_initcall(fn, 0)

#define core_initcall(fn)		__define_initcall(fn, 1)
#define postcore_initcall(fn)	__define_initcall(fn, 2)
#define arch_initcall(fn)		__define_initcall(fn, 3)
#define subsys_initcall(fn)		__define_initcall(fn, 4)
#define fs_initcall(fn)			__define_initcall(fn, 5)
#define rootfs_initcall(fn)		__define_initcall(fn, rootfs)
#define device_initcall(fn)		__define_initcall(fn, 6)
#define late_initcall(fn)		__define_initcall(fn, 7)

#define __initcall(fn) 			device_initcall(fn)

#endif
//---------------------------------------------------

#endif
