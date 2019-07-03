
	#include <std.h>
	#include <linkage.h>
	#include <traps.h>
	#include <entry.h>
	#include <cpu.h>

asmlinkage void addrerr_c(struct frame *fp);
asmlinkage void buserr_c(struct frame *fp);
asmlinkage void trap_c(struct frame *fp);
asmlinkage void syscall(struct frame *fp);

void murder_kernel(char *str, struct pt_regs *fp, int num);

static inline void bus_error030 (struct frame *fp){
	murder_kernel("Haulting execution: ", &fp->ptregs, 0);
}

asmlinkage void addrerr_c(struct frame *fp){
	printf("[!!!] Address Error, Format is %X\n", fp->ptregs.format);
	murder_kernel("Haulting execution: ", &fp->ptregs, 0);
}

asmlinkage void buserr_c(struct frame *fp){
	printf("[!!!] Bus Error, Format is %X\n", fp->ptregs.format);
	murder_kernel("Haulting execution", &fp->ptregs, 0);
}

void show_registers(struct pt_regs *regs)
{
	struct frame *fp = (struct frame *)regs;
	uint16_t c, *cp;
	uint32_t addr;
	int i;

	printf("PC: [%x]\n", regs->pc);
	printf("SR: %04x        a2: %08x\n", regs->sr, regs, regs->a2);
	printf("d0: %08x    d1: %08x    d2: %08x    d3: %08x\n",
	       regs->d0, regs->d1, regs->d2, regs->d3);
	printf("d4: %08x    d5: %08x    a0: %08x    a1: %08x\n",
	       regs->d4, regs->d5, regs->a0, regs->a1);

	addr = (uint32_t)&fp->un;
	printf("Frame format=%X ", regs->format);
	switch (regs->format) {
	case 0x2:
		printf("instr addr=%08x\n", fp->un.fmt2.iaddr);
		addr += sizeof(fp->un.fmt2);
		break;
	case 0x3:
		printf("eff addr=%08x\n", fp->un.fmt3.effaddr);
		addr += sizeof(fp->un.fmt3);
		break;
	case 0x4:
		printf("eff addr=%08x pc=%08x\n",
			fp->un.fmt4.effaddr, fp->un.fmt4.pc);
		addr += sizeof(fp->un.fmt4);
		break;
	case 0x7:
		printf("eff addr=%08x ssw=%04x faddr=%08x\n",
			fp->un.fmt7.effaddr, fp->un.fmt7.ssw, fp->un.fmt7.faddr);
		printf("wb 1 stat/addr/data: %04x %08x %08x\n",
			fp->un.fmt7.wb1s, fp->un.fmt7.wb1a, fp->un.fmt7.wb1dpd0);
		printf("wb 2 stat/addr/data: %04x %08x %08x\n",
			fp->un.fmt7.wb2s, fp->un.fmt7.wb2a, fp->un.fmt7.wb2d);
		printf("wb 3 stat/addr/data: %04x %08x %08x\n",
			fp->un.fmt7.wb3s, fp->un.fmt7.wb3a, fp->un.fmt7.wb3d);
		printf("push data: %08x %08x %08x %08x\n",
			fp->un.fmt7.wb1dpd0, fp->un.fmt7.pd1, fp->un.fmt7.pd2,
			fp->un.fmt7.pd3);
		addr += sizeof(fp->un.fmt7);
		break;
	case 0x9:
		printf("instr addr=%08x\n", fp->un.fmt9.iaddr);
		addr += sizeof(fp->un.fmt9);
		break;
	case 0xa:
		printf("ssw=%04x isc=%04x isb=%04x daddr=%08x dobuf=%08x\n",
			fp->un.fmta.ssw, fp->un.fmta.isc, fp->un.fmta.isb,
			fp->un.fmta.daddr, fp->un.fmta.dobuf);
		addr += sizeof(fp->un.fmta);
		break;
	case 0xb:
		printf("ssw=%04x isc=%04x isb=%04x daddr=%08x dobuf=%08x\n",
			fp->un.fmtb.ssw, fp->un.fmtb.isc, fp->un.fmtb.isb,
			fp->un.fmtb.daddr, fp->un.fmtb.dobuf);
		printf("baddr=%08x dibuf=%08x ver=%x\n",
			fp->un.fmtb.baddr, fp->un.fmtb.dibuf, fp->un.fmtb.ver);
		addr += sizeof(fp->un.fmtb);
		break;
	default:
		puts("[!!!] Bad frame format, unknown error");
	}

}

asmlinkage void trap_c(struct frame *fp){
	int vector = (fp->ptregs.vector >> 2) & 0xff;
	switch (vector){
		default:
			puts("[!!!] Illegal trap vector");
			break;
	}
}

void murder_kernel(char *str, struct pt_regs *fp, int num){
	printf("%s: %08X\n", str, num);
	show_registers(fp);
	while (1) asm ("");
}
