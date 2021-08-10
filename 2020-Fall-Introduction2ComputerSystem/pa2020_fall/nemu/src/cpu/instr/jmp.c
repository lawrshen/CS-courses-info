#include "cpu/instr.h"

make_instr_func(jmp_near)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, data_size);
        // thank Ting Xu from CS'17 for finding this bug
        print_asm_1("jmp", "", 1 + data_size / 8, &rel);

        cpu.eip += offset;
        return 1 + data_size / 8;
}

make_instr_func(jmp_short)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = 8;
        rel.addr = eip + 1;
        operand_read(&rel);
        int offset = sign_ext(rel.val, rel.data_size);
        print_asm_1("jmp", "", 2, &rel);
        cpu.eip += offset;
        return 2;
}

make_instr_func(jmp_near_indirect)
{
    int len=1;
	OPERAND rel;
	rel.data_size = data_size;
    rel.type = OPR_MEM;
    len+=modrm_rm(eip + 1, &rel);
	operand_read(&rel);
	print_asm_1("jmp", "",len , &rel);
	cpu.eip = rel.val;
	return 0;
}

make_instr_func(jmp_far_imm){
        OPERAND r16, r32;
        r16.type = OPR_IMM;
	    r16.sreg = SREG_CS;
        r16.data_size = 16;
        r16.addr = cpu.eip + 5;

        r32.type = OPR_IMM;
    	r32.sreg = SREG_CS;
        r32.data_size = 32;
        r32.addr = cpu.eip + 1;

        operand_read(&r32);
        operand_read(&r16);

        cpu.cs.val = r16.val;
        load_sreg(SREG_CS);
        cpu.eip = r32.val;

        return 0;
    
}