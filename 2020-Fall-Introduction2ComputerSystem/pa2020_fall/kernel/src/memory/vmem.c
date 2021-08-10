#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)
#define NR_PT ((SCR_SIZE + PT_SIZE - 1) / PT_SIZE) // number of page tables to cover the vmem

PDE *get_updir();
PTE uptable[1024] align_to_page; // define page table

void create_video_mapping()
{

	/* TODO: create an identical mapping from virtual memory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memeory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
    PDE *pdir = get_updir();
	PTE *ptable = (PTE *)va_to_pa(uptable);
	uint32_t pdir_idx, ptable_idx, pframe_idx;
	pframe_idx = 0;
	pdir_idx = VMEM_ADDR / PT_SIZE;
	pdir[pdir_idx].val = make_pde(ptable);
	pdir[pdir_idx + KOFFSET / PT_SIZE].val = make_pde(ptable);
	for (ptable_idx = 0; ptable_idx <= 0xaf; ptable_idx++)
	{
		ptable->val = make_pte(pframe_idx << 12);
		pframe_idx++;
		ptable++;
	}

	//panic("please implement me");
}

void video_mapping_write_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		buf[i] = i;
	}
}

void video_mapping_read_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		assert(buf[i] == i);
	}
}

void video_mapping_clear()
{
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}
