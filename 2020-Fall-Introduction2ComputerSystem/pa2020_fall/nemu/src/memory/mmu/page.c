#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
    uint32_t offset=laddr&0xfff;
    uint32_t page=(laddr>>12)&0x3ff;
    uint32_t dir=(laddr>>22)&0x3ff;
    
    uint32_t pde_addr=(cpu.cr3.pdbr<<12)+dir*4;
    PDE pde;
    memcpy(&pde,(void*)hw_mem+pde_addr,sizeof(pde));
    //pde.val = paddr_read(pde_addr, 4);
    
    if(pde.present!=1) {
		printf("eip = 0x%x, laddr = 0x%x, pde_addr = 0x%x, pde = 0x%x\n", cpu.eip, laddr, pde_addr , pde.val);
		fflush(stdout);
	}
	assert(pde.present==1);
    uint32_t pte_addr=(pde.page_frame<<12)+page*4;
    PTE pte;
    memcpy(&pte,(void*)hw_mem+pte_addr,sizeof(pte));
    //pte.val = paddr_read(pte_addr, 4);
    if(pte.present!=1) {
		printf("eip = 0x%x, laddr = 0x%x, pde.page_frame = 0x%x, pte = 0x%x\npde = 0x%x\n", cpu.eip, laddr, pde.page_frame, pte.val,pde.val);
		fflush(stdout);
	}
    assert(pte.present==1);
    uint32_t paddr=(pte.page_frame<<12)+offset;
    return paddr;
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
