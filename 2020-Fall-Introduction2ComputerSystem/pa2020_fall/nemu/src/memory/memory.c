#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "device/mm_io.h"
#include <memory.h>
#include <stdio.h>

uint8_t hw_mem[MEM_SIZE_B];

uint32_t hw_mem_read(paddr_t paddr, size_t len)
{
	uint32_t ret = 0;
	memcpy(&ret, hw_mem + paddr, len);
	return ret;
}

void hw_mem_write(paddr_t paddr, size_t len, uint32_t data)
{
	memcpy(hw_mem + paddr, &data, len);
}

uint32_t paddr_read(paddr_t paddr, size_t len) 
{
	uint32_t ret = 0;
    // #ifdef CACHE_ENABLED
    // 		ret = cache_read(paddr, len);     // 通过cache进行读
    // #else
    if(is_mmio(paddr)==-1)
        ret = hw_mem_read(paddr, len);
    else
        ret = mmio_read(paddr, len,is_mmio(paddr));
    //#endif
	return ret;
}

void paddr_write(paddr_t paddr, size_t len, uint32_t data) 
{
    // #ifdef CACHE_ENABLED
    // 		cache_write(paddr, len, data);    // 通过cache进行写
    // #else
    if(is_mmio(paddr)==-1)
		hw_mem_write(paddr, len, data);
    else
        mmio_write(paddr, len, data,is_mmio(paddr));
    //#endif
}

void init_mem()
{
	// clear the memory on initiation
	memset(hw_mem, 0, MEM_SIZE_B);
    #ifdef CACHE_ENABLED
    	init_cache();                             // 初始化cache
    #endif
    #ifdef TLB_ENABLED
    	make_all_tlb();
    	init_all_tlb();
    #endif
}

uint32_t laddr_read(laddr_t laddr, size_t len) {	
	assert(len == 1 || len == 2 || len == 4);	
	uint32_t paddr=laddr;
	#ifndef IA32_PAGE
	    paddr_read(laddr, len);
	    return paddr_read(paddr, len);	
	#else
    	if( cpu.cr0.pg==1  && cpu.cr0.pe==1) {
    	    if((laddr&0xfff)+len-1>0xfff)//data cross the page boundary
    	    {
    	        uint32_t l=(0xfff-(laddr&0xfff));
    	        uint32_t ret1=paddr_read(page_translate(laddr),l);
    	        uint32_t ret2=paddr_read(page_translate(laddr+l),len-l);
    	        return ret1+(ret2<<(8*l));
    	    }
    	    else{
    	        paddr = page_translate(laddr);
    	        return paddr_read(paddr, len);
    	    }
    	}
    	else
    	    return paddr_read(paddr, len);
    #endif
}

void laddr_write(laddr_t laddr, size_t len, uint32_t data) {	
	assert(len == 1 || len == 2 || len == 4);	
	uint32_t paddr=laddr;
	#ifndef IA32_PAGE
	    paddr_write(laddr, len, data);
	#else
	if( cpu.cr0.pg==1 && cpu.cr0.pe==1) {
	    if((laddr&0xfff)+len-1>0xfff)//data cross the page boundary
	    {
	        uint32_t l=(0xfff-(laddr&0xfff));
	        paddr_write(page_translate(laddr),l,(data&( (1<<8*l)-1) ) );
	        paddr_write(page_translate(laddr+l),len-l,data>>(8*l));
	        return ;
	    }
	    else{
	        paddr = page_translate(laddr);
	        paddr_write(paddr, len, data);
        	}
	}
	else
	    paddr_write(paddr, len, data);
	#endif
}

uint32_t vaddr_read(vaddr_t vaddr, uint8_t sreg, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	#ifndef IA32_SEG
        return laddr_read(vaddr, len);
    #else
        uint32_t laddr = vaddr;
        if( cpu.cr0.pe==1 ) {
            laddr = segment_translate(vaddr, sreg);
        }
        return laddr_read(laddr, len);
    #endif
}

void vaddr_write(vaddr_t vaddr, uint8_t sreg, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);
 	#ifndef IA32_SEG
        return laddr_write(vaddr, len, data);
    #else
        uint32_t laddr = vaddr;
        if( cpu.cr0.pe==1 ) {
            laddr = segment_translate(vaddr, sreg);
        }
        return laddr_write(laddr, len, data);
    #endif
	
}

uint32_t instr_fetch(vaddr_t vaddr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	return vaddr_read(vaddr, SREG_CS, len);
}

uint8_t *get_mem_addr()
{
	return hw_mem;
}
