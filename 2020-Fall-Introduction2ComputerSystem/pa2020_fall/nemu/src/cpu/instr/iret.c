#include "cpu/instr.h"
/*
Put the implementations of `iret' instructions here.
*/
make_instr_func(iret){
    print_asm_0("iret", "", 1);
   // printf("iret cpu.eip: %x\ncpu.esp: %x\n", cpu.eip,cpu.esp);
	cpu.eip=vaddr_read(cpu.esp,2,4);
//	printf("iret cpu.eip: %x\ncpu.esp: %x\n", cpu.eip,cpu.esp);
//	printf("iret cpu.eip: %x\n", cpu.eip);
	cpu.esp+=4;
	cpu.cs.val=vaddr_read(cpu.esp,2,2);
	cpu.esp+=4;
	cpu.eflags.val=vaddr_read(cpu.esp,2,4);
	cpu.esp+=4;
	print_asm_0("pusha", "", 1);
    return 0;
}