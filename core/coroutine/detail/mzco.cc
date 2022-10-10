#define _GNU_SOURCE

#include "mzco.h"
#include <stdio.h>
#include <stdint.h>

#include "assert_override.h"

void mz_runtime_test(void)
{
#ifdef __i386
    static_assert(sizeof(void*) == 4, "require 'sizeof(void*) == 4'");
#elif  __x86_64__
    static_assert(sizeof(void*) == 8, "require 'sizeof(void*) == 8'");
    static_assert(sizeof(__uint128_t) == 16, "require 'sizeof(__uint128_t) == 16'");
#else
    #error "platform no support yet"
#endif
    static_assert(sizeof(int) >= 4, "require 'sizeof(int) >= 4'");
    assert(sizeof(int) >= 4);
    static_assert(sizeof(int) <= sizeof(size_t),
            "require 'sizeof(int) <= sizeof(size_t)'");
    assert(sizeof(int) <= sizeof(size_t));
}

#define mz_amd64_inline_short_aligned_memcpy_test_ok(dst, src, sz) \
    ( \
      (((uintptr_t)(src) & 0x0f) == 0) && (((uintptr_t)(dst) & 0x0f) == 0) \
      && \
      (((sz) & 0x0f) == 0x08) && (((sz) >> 4) >= 0) && (((sz) >> 4) <= 8) \
      )

#define mz_amd64_inline_short_aligned_memcpy(dst, src, sz) do { \
    __uint128_t xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7; \
    switch((sz) >> 4) { \
        case 0: \
                break; \
        case 1: \
                xmm0 = *((__uint128_t*)(src) + 0);  \
                *((__uint128_t*)(dst) + 0) = xmm0; \
                break;  \
        case 2: \
                xmm0 = *((__uint128_t*)(src) + 0); \
                xmm1 = *((__uint128_t*)(src) + 1); \
                *((__uint128_t*)(dst) + 0) = xmm0; \
                *((__uint128_t*)(dst) + 1) = xmm1; \
                break; \
        case 3: \
                xmm0 = *((__uint128_t*)(src) + 0); \
                xmm1 = *((__uint128_t*)(src) + 1); \
                xmm2 = *((__uint128_t*)(src) + 2); \
                *((__uint128_t*)(dst) + 0) = xmm0; \
                *((__uint128_t*)(dst) + 1) = xmm1; \
                *((__uint128_t*)(dst) + 2) = xmm2; \
                break; \
        case 4: \
                xmm0 = *((__uint128_t*)(src) + 0); \
                xmm1 = *((__uint128_t*)(src) + 1); \
                xmm2 = *((__uint128_t*)(src) + 2); \
                xmm3 = *((__uint128_t*)(src) + 3); \
                *((__uint128_t*)(dst) + 0) = xmm0; \
                *((__uint128_t*)(dst) + 1) = xmm1; \
                *((__uint128_t*)(dst) + 2) = xmm2; \
                *((__uint128_t*)(dst) + 3) = xmm3; \
                break; \
        case 5: \
                xmm0 = *((__uint128_t*)(src) + 0); \
                xmm1 = *((__uint128_t*)(src) + 1); \
                xmm2 = *((__uint128_t*)(src) + 2); \
                xmm3 = *((__uint128_t*)(src) + 3); \
                xmm4 = *((__uint128_t*)(src) + 4); \
                *((__uint128_t*)(dst) + 0) = xmm0; \
                *((__uint128_t*)(dst) + 1) = xmm1; \
                *((__uint128_t*)(dst) + 2) = xmm2; \
                *((__uint128_t*)(dst) + 3) = xmm3; \
                *((__uint128_t*)(dst) + 4) = xmm4; \
                break; \
        case 6: \
                xmm0 = *((__uint128_t*)(src) + 0); \
                xmm1 = *((__uint128_t*)(src) + 1); \
                xmm2 = *((__uint128_t*)(src) + 2); \
                xmm3 = *((__uint128_t*)(src) + 3); \
                xmm4 = *((__uint128_t*)(src) + 4); \
                xmm5 = *((__uint128_t*)(src) + 5); \
                *((__uint128_t*)(dst) + 0) = xmm0; \
                *((__uint128_t*)(dst) + 1) = xmm1; \
                *((__uint128_t*)(dst) + 2) = xmm2; \
                *((__uint128_t*)(dst) + 3) = xmm3; \
                *((__uint128_t*)(dst) + 4) = xmm4; \
                *((__uint128_t*)(dst) + 5) = xmm5; \
                break; \
        case 7: \
                xmm0 = *((__uint128_t*)(src) + 0); \
                xmm1 = *((__uint128_t*)(src) + 1); \
                xmm2 = *((__uint128_t*)(src) + 2); \
                xmm3 = *((__uint128_t*)(src) + 3); \
                xmm4 = *((__uint128_t*)(src) + 4); \
                xmm5 = *((__uint128_t*)(src) + 5); \
                xmm6 = *((__uint128_t*)(src) + 6); \
                *((__uint128_t*)(dst) + 0) = xmm0; \
                *((__uint128_t*)(dst) + 1) = xmm1; \
                *((__uint128_t*)(dst) + 2) = xmm2; \
                *((__uint128_t*)(dst) + 3) = xmm3; \
                *((__uint128_t*)(dst) + 4) = xmm4; \
                *((__uint128_t*)(dst) + 5) = xmm5; \
                *((__uint128_t*)(dst) + 6) = xmm6; \
                break; \
        case 8: \
                xmm0 = *((__uint128_t*)(src) + 0); \
                xmm1 = *((__uint128_t*)(src) + 1); \
                xmm2 = *((__uint128_t*)(src) + 2); \
                xmm3 = *((__uint128_t*)(src) + 3); \
                xmm4 = *((__uint128_t*)(src) + 4); \
                xmm5 = *((__uint128_t*)(src) + 5); \
                xmm6 = *((__uint128_t*)(src) + 6); \
                xmm7 = *((__uint128_t*)(src) + 7); \
                *((__uint128_t*)(dst) + 0) = xmm0; \
                *((__uint128_t*)(dst) + 1) = xmm1; \
                *((__uint128_t*)(dst) + 2) = xmm2; \
                *((__uint128_t*)(dst) + 3) = xmm3; \
                *((__uint128_t*)(dst) + 4) = xmm4; \
                *((__uint128_t*)(dst) + 5) = xmm5; \
                *((__uint128_t*)(dst) + 6) = xmm6; \
                *((__uint128_t*)(dst) + 7) = xmm7; \
                break; \
    }\
    *((uint64_t*)((uintptr_t)(dst) + (sz) - 8)) = *((uint64_t*)((uintptr_t)(src) + (sz) - 8)); \
} while (0)

#define mz_amd64_optimized_memcpy_drop_in(dst, src, sz) do {\
    if (mz_amd64_inline_short_aligned_memcpy_test_ok((dst), (src), (sz))) { \
        mz_amd64_inline_short_aligned_memcpy((dst), (src), (sz)); \
    } else { \
        memcpy((dst), (src), (sz)); \
    } \
} while (0)

static void mz_default_protector_last_word(void) {
    mz_t* co = mz_get_co();
    fprintf(stderr, "error: mz_default_protector_last_word trigger\n");
    fprintf(stderr, "errpr: co:%p should call `mz_exit()` instead of direct"
            "`return` in co_fp:%p to finishi its execution\n", co, (void*)co->fp);
    assert(0);
}

__thread mz_t* mz_gtls_co;
static __thread mz_cofuncp_t mz_gtls_last_word_fp = mz_default_protector_last_word;

#ifdef __i386__
    static __thread void* mz_gtls_fpucw_mxcsr[2];
#elif __x86_64__
    static __thread void* mz_gtls_fpucw_mxcsr[1];
#else
    #error "platform no support yet"
#endif

void mz_thread_init(mz_cofuncp_t last_word_co_fp)
{
    mz_save_fpucw_mxcsr(mz_gtls_fpucw_mxcsr);
    if ((void*)last_word_co_fp != NULL)
    {
        mz_gtls_last_word_fp = last_word_co_fp;
    }
}

void mz_funcp_protector(void)
{
    if ((void*)(mz_gtls_last_word_fp) != NULL)
    {
        mz_gtls_last_word_fp();
    }
    else {
        mz_default_protector_last_word();
    }
    assert(0);
}

mz_share_stack_t* mz_share_stack_new(size_t sz)
{
    return mz_share_stack_new2(sz, 1);
}

#define mz_size_t_safe_add_assert(a, b) do { \
    assert((a) + (b) >= (a)); \
} while (0)

mz_share_stack_t* mz_share_stack_new2(size_t sz, char guard_page_enabled)
{
    if (sz == 0)
    {
        sz = 1024 * 1024 * 2;
    }
    if (sz < 4096)
    {
        sz = 4096;
    }
    assert(sz > 0);

    size_t u_pgsz = 0;

    if (guard_page_enabled != 0)
    {
        long pgsz = sysconf(_SC_PAGESIZE);
        assert(pgsz > 0 && (((pgsz - 1) & pgsz) == 0));
        u_pgsz = (size_t)((unsigned long)pgsz);
        assert(u_pgsz == (unsigned long)pgsz && ((u_pgsz << 1) >> 1) == u_pgsz);
        if (sz <= u_pgsz) {
            sz = u_pgsz << 1;
        }
        else 
        {
            size_t new_sz;
            if ((sz & (u_pgsz - 1) != 0))
            {
                new_sz = (sz & (~(u_pgsz - 1)));
                assert(new_sz >= u_pgsz);
                mz_size_t_safe_add_assert(new_sz, (u_pgsz << 1));
                new_sz = new_sz + (u_pgsz << 1);
                assert(sz / u_pgsz + 2 == new_sz / u_pgsz);
            }
            else 
            {
                mz_size_t_safe_add_assert(sz, u_pgsz);
                new_sz = sz + u_pgsz;
                assert(sz / u_pgsz + 1 == new_sz / u_pgsz);
            }
            sz = new_sz;
            assert((sz / u_pgsz > 1) && ((sz & (u_pgsz - 1)) == 0));
        }
    }

    mz_share_stack_t* p = (mz_share_stack_t*)malloc(sizeof(mz_share_stack_t));
    assertalloc_ptr(p);
    memset(p, 0, sizeof(mz_share_stack_t));

    if (guard_page_enabled != 0)
    {
        p->real_ptr = mmap(NULL, sz, PROT_READ 
                | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        assertalloc_bool(p->real_ptr != MAP_FAILED);
        p->guard_page_enabled = 1;
        assert(0 == mprotect(p->real_ptr, u_pgsz, PROT_READ));

        p->ptr = (void*)(((uintptr_t)p->real_ptr) + u_pgsz);
        p->real_sz = sz;
        assert(sz >= (u_pgsz << 1));
        p->sz = sz - u_pgsz;
    } 
    else
    {
        p->sz = sz;
        p->ptr = malloc(sz);
        assertalloc_ptr(p->ptr);
    }

    p->owner = NULL;

#ifdef MZ_USE_VALGRIND
    p->valgrind_stk_id = VALGRIND_STACK_REGISTER(
            p->ptr, (void*)((uintptr_t)p->ptr + p->sz));
#endif
#if defined(__i386) || defined(__x86_64__)
    uintptr_t u_p = (uintptr_t)(p->sz - (sizeof(void*) << 1) + (uintptr_t)p->ptr);
    u_p = (u_p >> 4) << 4;
    p->align_highptr = (void*)u_p;
    p->align_retptr = (void*)(u_p - sizeof(void*));
    *((void**)(p->align_retptr)) = (void*)(mz_funcp_protector_asm);
    assert(p->sz > (16 + (sizeof(void*) << 1) + sizeof(void*)));
    p->align_limit = p->sz - 16 - (sizeof(void*) << 1);
#else
#error "platform no support yet"
#endif
    return p;
}

void mz_share_stack_destory(mz_share_stack_t* sstk)
{
    assert(sstk != NULL && sstk->ptr != NULL);
#ifdef MZ_USE_VALGRIND
    VALGRIND_STACK_DEREGISTER(sstk->valgrind_stk_id);
#endif
    if (sstk->guard_page_enabled)
    {
        assert(0 == munmap(sstk->real_ptr, sstk->real_sz));
        sstk->real_ptr = NULL;
        sstk->ptr = NULL;
    }
    else 
    {
        free(sstk->ptr);
        sstk->ptr = NULL;
    }
    free(sstk);
}

mz_t* mz_create(mz_t* main_co, mz_share_stack_t* share_stack,
        size_t save_stack_sz, mz_cofuncp_t fp, void* arg)
{
    mz_t* p = (mz_t*)malloc(sizeof(mz_t));
    assertalloc_ptr(p);
    memset(p, 0, sizeof(mz_t));

    if (main_co != NULL)
    {

        assertptr(share_stack);
        p->share_stack = share_stack;
#ifdef __i386__
        p->reg[MZ_REG_IDX_RETADDR] = (void*)fp;
        p->reg[MZ_REG_IDX_SP] = p->share_stack->align_retptr;
    #ifndef MZ_CONFIG_SHARE_FPU_MXCSR_ENV
        p->reg[MZ_REG_IDX_FPU] = mz_gtls_fpucw_mxcsr[0];
        p->reg[MZ_REG_IDX_FPU + 1] = mz_gtls_fpucw_mxcsr[1];
    #endif
#elif __x86_64__
        p->reg[MZ_REG_IDX_RETADDR] = (void*)fp;
        p->reg[MZ_REG_IDX_SP] = p->share_stack->align_retptr;
    #ifndef MZ_CONFIG_SHARE_FPU_MXCSR_ENV
        p->reg[MZ_REG_IDX_FPU] = mz_gtls_fpucw_mxcsr[0];
    #endif
#else
    #error "platform no support yet"
#endif
        p->main_co = main_co;
        p->arg = arg;
        p->fp = fp;
        if (save_stack_sz == 0)
        {
            save_stack_sz = 64;
        }
        p->save_stack.ptr = malloc(save_stack_sz);
        assertalloc_ptr(p->save_stack.ptr);
        p->save_stack.sz = save_stack_sz;
#if defined(__i386__) || defined(__x86_64__)
        p->save_stack.valid_sz = 0;
#else
    #error "platform no support yet"
#endif
        return p;
    }
    else
    {
        p->main_co = NULL;
        p->arg = arg;
        p->fp = fp;
        p->share_stack = NULL;
        p->save_stack.ptr = NULL;
        printf("success1\n");
        return p;
    }
    assert(0);
}

mz_attr_no_asan
void mz_resume(mz_t* resume_co)
{
    assert(resume_co != NULL && resume_co->main_co != NULL
            && resume_co->is_end == 0);
    if (resume_co->share_stack->owner != resume_co)
    {
        if (resume_co->share_stack->owner != NULL)
        {
            mz_t* owner_co = resume_co->share_stack->owner;
            assert(owner_co->share_stack == resume_co->share_stack);
#if defined(__i386__) || defined(__x86_64__)
            assert(((uintptr_t)(owner_co->share_stack->align_retptr)
                        >= (uintptr_t)(owner_co->reg[MZ_REG_IDX_SP]))
                    && ((uintptr_t)(owner_co->share_stack->align_highptr)
                        - (uintptr_t)(owner_co->share_stack->align_limit)
                        <= (uintptr_t)(owner_co->reg[MZ_REG_IDX_SP])));
            owner_co->save_stack.valid_sz = 
                (uintptr_t)(owner_co->share_stack->align_retptr)
                - (uintptr_t)(owner_co->reg[MZ_REG_IDX_SP]);
            if (owner_co->save_stack.sz < owner_co->save_stack.valid_sz)
            {
                free(owner_co->save_stack.ptr);
                owner_co->save_stack.ptr = NULL;
                while (1)
                {
                    owner_co->save_stack.sz = owner_co->save_stack.sz << 1;
                    assert(owner_co->save_stack.sz > 0);
                    if (owner_co->save_stack.sz >= owner_co->save_stack.valid_sz)
                    {
                        break;
                    }
                }
                owner_co->save_stack.ptr = malloc(owner_co->save_stack.sz);
                assertalloc_ptr(owner_co->save_stack.ptr);
            }
            if (owner_co->save_stack.valid_sz > 0)
            {
    #ifdef __x86_64__
                mz_amd64_optimized_memcpy_drop_in(
                        owner_co->save_stack.ptr,
                        owner_co->reg[MZ_REG_IDX_SP],
                        owner_co->save_stack.valid_sz);
    #else
                memcpy(
                        owner_co->save_stack.ptr,
                        owner_co->reg[MZ_REG_IDX_SP],
                        owner_co->save_stack.valid_sz);
    #endif
                owner_co->save_stack.ct_save++;
            }
            if (owner_co->save_stack.valid_sz > owner_co->save_stack.max_cpsz)
            {
                owner_co->save_stack.max_cpsz = owner_co->save_stack.valid_sz;
            }
            owner_co->share_stack->owner = NULL;
            owner_co->share_stack->align_validsz = 0;
#else
    #error "platform no support yet"
#endif
        }
        assert(resume_co->share_stack->owner == NULL);
#if defined(__i386__) || defined(__x86_64__)
        assert(resume_co->save_stack.valid_sz 
                <= resume_co->share_stack->align_limit - sizeof(void*));
        if (resume_co->save_stack.valid_sz > 0)
        {
        #ifdef __x86_64__
            mz_amd64_optimized_memcpy_drop_in(
                    (void*)((uintptr_t)(resume_co->share_stack->align_retptr)
                        - resume_co->save_stack.valid_sz),
                    resume_co->save_stack.ptr,
                    resume_co->save_stack.valid_sz);
        #else
            memcpy(
                    (void*)((uintptr_t)(resume_co->share_stack->align_retptr)
                        - resume_co->save_stack.valid_sz),
                    resume_co->save_stack.ptr,
                    resume_co->save_stack.valid_sz);
        #endif
            resume_co->save_stack.ct_restore++;
        }
        if (resume_co->save_stack.valid_sz > resume_co->save_stack.max_cpsz)
        {
            resume_co->save_stack.max_cpsz = resume_co->save_stack.valid_sz;
        }
        resume_co->share_stack->align_validsz = 
            resume_co->save_stack.valid_sz + sizeof(void*);
        resume_co->share_stack->owner = resume_co;
#else
    #error "platform no support yet"
#endif
    }
    mz_gtls_co = resume_co;
    mzswap(resume_co->main_co, resume_co);
    mz_gtls_co = resume_co->main_co;
}

void mz_destory(mz_t* co)
{
    assertptr(co);
    if (mz_is_main_co(co))
    {
        free(co);
    }
    else
    {
        if (co->share_stack->owner == co)
        {
            co->share_stack->owner = NULL;
            co->share_stack->align_validsz = 0;
        }
        free(co->save_stack.ptr);
        co->save_stack.ptr = NULL;
        free(co);
    }
}

