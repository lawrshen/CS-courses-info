#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/
static void instr_execute_1op() 
{
	opr_dest.type=OPR_MEM;
	opr_dest.data_size=data_size;
	opr_dest.sreg=SREG_DS;
	opr_dest.addr=cpu.esp;
	operand_read(&opr_dest);

	opr_src.val=opr_dest.val;
	operand_write(&opr_src);
	cpu.esp+=data_size/8;
}

make_instr_impl_1op(pop,r,v)

make_instr_func(popa){
    opr_dest.data_size = data_size;
    opr_dest.type = OPR_MEM;
    opr_dest.sreg = SREG_SS;
    for (int i = 7; i >= 0; i--)
    {
        opr_dest.addr = cpu.esp;
        if(i != REG_ESP){
            operand_read(&opr_dest);
            cpu.gpr[i].val = opr_dest.val;
        }
        cpu.esp += data_size >> 3;
    }
    print_asm_0("popa", "", 1);
    return 1;
}

