#include "cpu/instr.h"
/*
Put the implementations of `ret' instructions here.
*/
make_instr_func(ret_near)
{
        print_asm_0("ret", "", 1);
        OPERAND mem;
        mem.type = OPR_MEM;
        mem.sreg = SREG_DS;
        mem.data_size = data_size;
        mem.addr = cpu.esp ;
        operand_read(&mem);
        cpu.eip=mem.val;
        cpu.esp+=data_size/8;
        return 0;
}
make_instr_func(ret_near_imm16)
{
        print_asm_0("ret", "", 3);
        OPERAND mem;
        mem.type = OPR_MEM;
        mem.sreg = SREG_DS;
        mem.data_size = data_size;
        mem.addr = cpu.esp ;
        operand_read(&mem);
        cpu.eip=mem.val;
        int offset=sign_ext(instr_fetch(eip+1,2),16);
        cpu.esp+=data_size/8+offset;
        return 0;
}