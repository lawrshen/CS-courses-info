#include "cpu/instr.h"
/*
Put the implementations of `leave' instructions here.
*/
make_instr_func(leave){
    cpu.esp=cpu.ebp;
	opr_dest.type=OPR_MEM;
	opr_dest.data_size=data_size;
	opr_dest.addr=cpu.esp;
	operand_read(&opr_dest);
	cpu.ebp=opr_dest.val;
	cpu.esp+=data_size/8;
	return 1;
}
