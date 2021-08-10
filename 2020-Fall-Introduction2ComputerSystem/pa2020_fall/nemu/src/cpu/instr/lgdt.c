#include "cpu/instr.h"
/*
Put the implementations of `lgdt' instructions here.
*/
make_instr_func(lgdt){
    int len=1;
    opr_src.data_size=16; //m16
    opr_dest.data_size=32;//m32
    len+=modrm_rm(eip+1,&opr_src);
    opr_dest.type=opr_src.type;
    opr_dest.addr=opr_src.addr+2; 
    operand_read(&opr_src);
    operand_read(&opr_dest);
    
    print_asm_1("lgdt","",6,&opr_src);
    cpu.gdtr.limit=opr_src.val;
    cpu.gdtr.base = (data_size==16) ? (opr_dest.val&0xffffff) : opr_dest.val;
    return len;
}