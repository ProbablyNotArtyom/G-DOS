
	#include <entry.h>
	#include <asm.h>
	#include <linkage.h>
	#include <cpu.h>

	.section ".got","aw"
	 .global	__GOT_START__
	.type	__GOT_START__,@object
__GOT_START__:

	.section ".got1","aw"
	 .global	__GOT1_START__
	.type	__GOT1_START__,@object
__GOT1_START__:

	.section ".got2","aw"
	 .global	__GOT2_START__
	.type	__GOT2_START__,@object
__GOT2_START__:

	.section ".fixup","aw"
	 .global	__FIXUP_START__
	.type	__FIXUP_START__,@object
__FIXUP_START__:

	.section ".ctors","aw"
	 .global	__CTOR_LIST__
	.type	__CTOR_LIST__,@object
__CTOR_LIST__:

	.section ".dtors","aw"
	 .global	__DTOR_LIST__
	.type	__DTOR_LIST__,@object
__DTOR_LIST__:

	.section ".sdata","aw"
	 .global	__SDATA_START__
	.type	__SDATA_START__,@object
	.weak	_SDA_BASE_
	.type	_SDA_BASE_,@object
__SDATA_START__:
_SDA_BASE_:

	.section ".sbss","aw",@nobits
	 .global	__SBSS_START__
	.type	__SBSS_START__,@object
__SBSS_START__:

	.section ".sdata2","a"
	.weak	_SDA2_BASE_
	.type	_SDA2_BASE_,@object
	 .global	__SDATA2_START__
	.type	__SDATA2_START__,@object
__SDATA2_START__:
_SDA2_BASE_:

	.section ".sbss2","a"
	 .global	__SBSS2_START__
	.type	__SBSS2_START__,@object
__SBSS2_START__:

	.section ".gcc_except_table","aw"
	 .global	__EXCEPT_START__
	.type	__EXCEPT_START__,@object
__EXCEPT_START__:

	.section ".eh_frame","aw"
	 .global	__EH_FRAME_BEGIN__
	.type	__EH_FRAME_BEGIN__,@object
__EH_FRAME_BEGIN__:
