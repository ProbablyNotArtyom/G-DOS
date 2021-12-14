
	#include <entry.h>
	#include <sys/cdefs.h>
	#include <linkage.h>

# Converts the filesystem image into a linkable object by wrapping it in assembly
# Normally you would convert the binary directly to an object using objcopy
# However, certaing GCC targets (notably arm) will refuse to link to objects with unknown ABI types
# Since objcopy defaults to the "none" ABI and provides no way to change it, these targets will fail to link
# Using this wrapper instead allows us to force the use of the correct ABI type

.section .data

_binary___romdisk_img_start:
	.incbin "romdisk.img"
_binary___romdisk_img_end:

.global _C_LABEL (_binary___romdisk_img_start)
.global _C_LABEL (_binary___romdisk_img_end)
.global _C_LABEL (_binary___romdisk_img_size)
.set _C_LABEL (_binary___romdisk_img_size),_binary___romdisk_img_end - _binary___romdisk_img_start
