#include "memory/mmu/cache.h"
#include "memory/memory.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
CacheLine CaChe[1024];
// init the cache
void init_cache()
{
    for(int i=0;i<1024;i++)
        CaChe[i].valid_bit = 0;
	// implement me in PA 3-1
}

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data)
{
    /*write through*/
    memcpy(hw_mem+paddr,&data,len);
	/*memory addr：tag —— array_num —— array_addr*/
    uint32_t tag=(paddr>>13)&0x7ffff;   //32-7-6
    uint32_t array_num=(paddr>>6)&0x7f; //2^10行->2^3行/组->2^7组
    uint32_t array_addr=paddr&0x3f;     //64B 2^6
    /*check array_num to find this cache whether be able to write*/
    uint32_t begin=array_num*8;
    uint32_t end=begin+8;
    int i=0;
    for(i=begin;i<end;i++){
        if(CaChe[i].tag==tag&&CaChe[i].valid_bit==1){//hit
            if(array_addr+len<=64)
                memcpy(CaChe[i].data+array_addr, &data, len);
             else{//cross line       array_addr <-- 64-array_addr -->| array_addr+len-64
                cache_write(paddr,64-array_addr,data);
                cache_write(paddr+ 64-array_addr,array_addr+len-64,data>>(8*(64-array_addr)));
			}
			break;
        }
    }
}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len)
{   
    uint32_t ret = 0;
    /*memory addr：tag —— array_num —— array_addr*/
    uint32_t tag=(paddr>>13)&0x7ffff;   //32-7-6
    uint32_t array_num=(paddr>>6)&0x7f; //2^10行->2^3行/组->2^7组
    uint32_t array_addr=paddr&0x3f;     //64B 2^6
    /*array_num decide cache where to read*/
    uint32_t begin=array_num*8;
    uint32_t end=begin+8;
    int i=0;
    for(i=begin;i<end;i++){
        if(CaChe[i].tag==tag&&CaChe[i].valid_bit==1){//hit
            if(array_addr+len<=64){
                memcpy(&ret,CaChe[i].data+array_addr,len);
                return ret;
            }
            else{//cross line       array_addr <-- 64-array_addr -->| array_addr+len-64
                uint32_t temp1=0,temp2=0;
				memcpy(&temp1,CaChe[i].data+array_addr,64-array_addr);
				temp2=cache_read(paddr+64-array_addr,array_addr+len-64)<<(8*(64-array_addr));
				ret=temp2|temp1;
				return ret;
            }
        }
    }
    if(i==end){//over but not find
        memcpy(&ret,hw_mem+paddr,len);
        /*replace cache*/
        for(int i=begin;i<end;i++)
            if(CaChe[i].valid_bit==0){
                CaChe[i].valid_bit=1;
                CaChe[i].tag=tag;
                memcpy(CaChe[i].data,hw_mem+paddr-array_addr,64);//64B
                return ret;
            }
        /*random replace*/
        srand((unsigned)time(0));
		i=begin+rand()%8;
        CaChe[i].tag=tag;
        memcpy(CaChe[i].data,hw_mem+paddr-array_addr,64);//64B
        return ret;
    }
    return 0;
}

