#include "cpu/instr.h"
/*
Put the implementations of `lidt' instructions here.
*/
make_instr_func(lidt){
    int len=1;
	opr_src.data_size=16; //m16
    opr_dest.data_size=32;//m32
    len+=modrm_rm(eip+1,&opr_src);
    opr_dest.type=opr_src.type;
    opr_dest.addr=opr_src.addr+2; 
    operand_read(&opr_src);
    operand_read(&opr_dest);
    
    print_asm_1("lidt","",6,&opr_src);
    cpu.idtr.limit=opr_src.val;
    cpu.idtr.base = (data_size==16) ? (opr_dest.val&0xffffff) : opr_dest.val;
    return len;
}