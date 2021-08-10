#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

void push_eflags(){
	cpu.esp-=4;
	vaddr_write(cpu.esp,2,4,cpu.eflags.val);
}
void push_cs(){
	cpu.esp-=4;
	vaddr_write(cpu.esp,2,4,(cpu.cs.val&0xffff));
}
void push_eip(){
	cpu.esp-=4;
	vaddr_write(cpu.esp,2,4,cpu.eip);
}

void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
    push_eflags();
    push_cs();
    push_eip();
//     //push eflags,cs,eip
// 	OPERAND rel;
// 	//eflags
// 	cpu.esp-=4;
// 	rel.data_size=32;
// 	rel.sreg=SREG_SS;
// 	rel.type=OPR_REG;
// 	rel.addr=cpu.esp;
// 	rel.val =cpu.eflags.val;
// 	operand_write(&rel);
// 	//cs
// 	cpu.esp-=4;
// 	rel.data_size=16;
// 	rel.addr=cpu.esp;
// 	rel.val =cpu.cs.val;
// 	operand_write(&rel);
// 	//eip
// 	cpu.esp-=4;
// 	rel.data_size=32;
// 	rel.addr=cpu.esp;
// 	rel.val =cpu.eip;
// 	operand_write(&rel);
// 	OPERAND temp;
// 	temp.data_size=data_size;
// 	temp.type = OPR_MEM;
//     temp.sreg = SREG_SS;
// 	temp.addr=cpu.esp;
// 	operand_read(&temp);
 //	printf("intr cpu.eip: %x\ncpu.esp: %x  temp:%x\n", cpu.eip,cpu.esp,temp.val);
// 	//find IDT
	GateDesc gate;
	gate.val[0]=laddr_read((cpu.idtr.base+intr_no*8),4);
	gate.val[1]=laddr_read(((cpu.idtr.base+intr_no*8)+4),4);
	cpu.eip=gate.offset_15_0|(gate.offset_31_16<<16);
	cpu.cs.val=gate.selector;
	if(gate.type==0xe)
		cpu.eflags.IF=0;
#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	//printf("intr cpu.eip : %x\n", cpu.eip);
	raise_intr(intr_no);
//	printf("intr cpu.eip : %x\n", cpu.eip);
}
