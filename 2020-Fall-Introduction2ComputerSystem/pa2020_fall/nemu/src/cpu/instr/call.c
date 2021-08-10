#include "cpu/instr.h"
/*
Put the implementations of `call' instructions here.
*/

make_instr_func(call_near)
{
    opr_src.type = OPR_IMM;
    opr_src.sreg = SREG_CS;
    opr_src.data_size = data_size;
    opr_src.addr = cpu.eip + 1;

    operand_read(&opr_src);

    print_asm_1("call", "", 1 + data_size / 8, &opr_src);
    cpu.esp-=data_size/8;
    opr_dest.type = OPR_MEM;
    opr_dest.sreg = SREG_DS;
    opr_dest.data_size = data_size;
    opr_dest.addr = cpu.esp ;
    opr_dest.val=cpu.eip+1+data_size/8;
    operand_write(&opr_dest);
    int offset=sign_ext(opr_src.val,data_size);
    cpu.eip+=offset+1+data_size/8;

    return 0;
}

make_instr_func(call_near_indirect){
    opr_src.type = OPR_IMM;
   // opr_src.sreg = SREG_CS;
    opr_src.data_size = data_size;
    opr_src.addr = cpu.eip + 1;
    int len=1;
    len+=modrm_rm(cpu.eip + 1, &opr_src);
	operand_read(&opr_src);
	print_asm_1("call", "",len , &opr_src);
	cpu.esp-=data_size/8;
    opr_dest.type = OPR_MEM;
    opr_dest.sreg = SREG_DS;
    opr_dest.data_size = data_size;
    opr_dest.addr = cpu.esp ;
    opr_dest.val=cpu.eip+len;
    operand_write(&opr_dest);
	cpu.eip = opr_src.val;
	return 0;
}