#include "cpu/instr.h"
/*
Put the implementations of `push' instructions here.
*/

static void instr_execute_1op() 
{
	operand_read(&opr_src);
	cpu.esp-=data_size/8;
	opr_dest.type=OPR_MEM;
	opr_dest.data_size=data_size;
	opr_dest.sreg=SREG_DS;
	opr_dest.addr=cpu.esp;
	opr_dest.val=opr_src.val;
	operand_write(&opr_dest);
}

make_instr_impl_1op(push,r,v)
make_instr_impl_1op(push,rm,v)
make_instr_impl_1op(push,i,b)
make_instr_impl_1op(push,i,v)

make_instr_func(pusha){
    uint32_t temp=cpu.esp;
    opr_dest.data_size = data_size;
    opr_dest.type = OPR_MEM;
    opr_dest.sreg = SREG_SS;

    for (int i = 0; i < 8; i++)
    {
        temp -= data_size >> 3;
        opr_dest.addr = temp;
        opr_dest.val = cpu.gpr[i].val;
        operand_write(&opr_dest);
    }
    cpu.esp = temp;
    print_asm_0("pusha", "", 1);
    return 1;
}
