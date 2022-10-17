#include "../mzco.h"
#include <alloca.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "assert_override.h"

// global coroutine function ptr
mz_cofuncp_t gl_co_fp;

#define PRINT_BUF_SZ 64
char gl_benchmark_print_str_buf[PRINT_BUF_SZ];

void co_fp_alloca()
{
    size_t sz = (size_t)((uintptr_t)mz_get_arg());
    uint8_t* ptr = NULL;
    assert(sz > 0);
    ptr = alloca(sz);
    assertptr(ptr);
    memset(ptr, 0, sz);
    while (1) 
    {
        mz_yield();
    }
    mz_exit();
}


void co_fp_stksz_128(){
    int ip[28];
    memset(ip, 1, sizeof(ip));
    while(1){
        aco_yield();
    }
    aco_exit();
}

void co_fp_stksz_64(){
    int ip[12];
    memset(ip, 1, sizeof(ip));
    while(1){
        aco_yield();
    }
    aco_exit();
}

void co_fp_stksz_40(){
    int ip[8];
    memset(ip, 1, sizeof(ip));
    while(1){
        aco_yield();
    }
    aco_exit();
}

void co_fp_stksz_24(){
    int ip[4];
    memset(ip, 1, sizeof(ip));
    while(1){
        aco_yield();
    }
    aco_exit();
}

void co_fp_stksz_8(){
    while(1){
        aco_yield();
    }
    aco_exit();
}

void co_fp0(){
    while(1){
        aco_yield();
    }
    aco_exit();
}

void benchmark_copystack(size_t co_amount, size_t stksz, int loopct)
{
    
}
