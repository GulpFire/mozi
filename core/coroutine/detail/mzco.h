#ifndef MOZI_CORE_COROUTINE_DETAIL_MZ_CO_H
#define MOZI_CORE_COROUTINE_DETAIL_MZ_CO_H

#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>

#ifdef MZ_USE_VALGRIND
#include <valgrind/valgrind.h>
#endif

extern "C" {

#define MOZI_VERSION_MAJOR  0
#define MOZI_VERSION_MINOR  0
#define MOZI_VERSION_PATCH  0

#ifdef __i386__
    #define MZ_REG_IDX_RETADDR  0
    #define MZ_REG_IDX_SP       1
    #define MZ_REG_IDX_BP       2
    #define MZ_REG_IDX_FPU      6
#elif  __x86_64__
    #define MZ_REG_IDX_RETADDR  4
    #define MZ_REG_IDX_SP       5
    #define MZ_REG_IDX_BP       7
    #define MZ_REG_IDX_FPU      8
#else
    #error "platform no support yet"
#endif

// save stack
typedef struct {
    void* ptr;
    size_t sz;
    size_t valid_sz;
    size_t max_cpsz;
    size_t ct_save;
    size_t ct_restore;
} mz_save_stack_t;

struct mz_s;
typedef struct mz_s mz_t;

// share stack
typedef struct
{
    void* ptr;
    size_t sz;
    void* align_highptr;
    void* align_retptr;
    size_t align_validsz;
    size_t align_limit;
    mz_t* owner;

    char guard_page_enabled;
    void* real_ptr;
    size_t real_sz;

#ifdef MZ_USE_VALGRIND
    unsigned long valgrind_stk_id;
#endif
} mz_share_stack_t;

typedef void (*mz_cofuncp_t)(void);

struct mz_s {
#ifdef __i386__
    #ifdef MZ_CONFIG_SHARE_FPU_MXCSR_ENV
        void* reg[6];
    #else
        void* reg[8];
    #endif
#elif __x86_64__
    #ifdef MZ_CONFIG_SHARE_FPU_MXCSR_ENV
        void* reg[8];
    #else
        void* reg[9];
    #endif
#else
    #error "platform no support yet"
#endif
    mz_t* main_co;
    void* arg;
    char is_end;
    mz_cofuncp_t fp;
    mz_save_stack_t save_stack;
    mz_share_stack_t* share_stack;
};

#define mz_likely(x)    (__builtin_expect(!!(x), 1))

#define mz_unlikely(x)  (__builtin_expect(!!(x), 0))

#define mz_assert(EX)   ((mz_likely(EX))?((void)0):(abort()))

#define mz_assertptr(ptr) ((mz_likely((ptr) != NULL))?((void)0):(abort()))

#define mz_assertalloc_bool(b) do { \
    if (mz_unlikely(!(b))) { \
        fprintf(stderr, "Aborting: failed to allocate memory: %s:%d:%s\n", \
                __FILE__, __LINE__, __PRETTY_FUNCTION__);   \
        abort();    \
    }   \
} while(0)

#define mz_assertalloc_ptr(ptr) do { \
    if(mz_unlikely((ptr) == NULL)){    \
        fprintf(stderr, "Aborting: failed to allocate memory: %s:%d:%s\n", \
            __FILE__, __LINE__, __PRETTY_FUNCTION__);    \
        abort();    \
    }   \
} while(0)

#if defined(mz_attr_no_asan)
    #error "mz_attr_no_asan already defined"
#endif
#if defined(MZ_USE_ASAN)
    #if defined(__has_feature)
        #if __has_feature(__address_sanitizer__)
            #define mz_attr_no_asan \
                __attribute__((__no_sanitize_address__))
        #endif
    #endif
    #if defined(__SANITIZE_ADDRESS__) && !defined(mz_attr_no_asan)
        #define mz_attr_no_asan \
            __attribute__((__no_sanitize_address__))
    #endif
#endif
#ifndef mz_attr_no_asan
    #define mz_attr_no_asan
#endif

extern void mz_runtime_test(void);

extern void mz_thread_init(mz_cofuncp_t last_word_co_fp);

extern void* mzswap(mz_t* from_co, mz_t* to_co) __asm__("mzswap");  // asm

extern void mz_save_fpucw_mxcsr(void* p) __asm__("mz_save_fpucw_mxcsr"); // asm

extern void mz_funcp_protector_asm(void) __asm__("mz_funcp_protector_asm"); // asm

extern void mz_funcp_protector(void);

extern mz_share_stack_t* mz_share_stack_new(size_t sz);

mz_share_stack_t* mz_share_stack_new2(size_t sz, char guard_page_enabled);

extern void mz_share_stack_destroy(mz_share_stack_t* sstk);

extern mz_t* mz_create(mz_t* main_co, mz_share_stack_t* share_stack,
        size_t save_stack_sz, mz_cofuncp_t fp, void* arg);

// mz's Global Thread Local Storage variable 'co'
extern __thread mz_t* mz_gtls_co;

mz_attr_no_asan
extern void mz_resume(mz_t* resume_co);

#define mz_yield1(yield_co) do { \
    mz_assertptr(yield_co); \
    mz_assertptr((yield_co)->main_co);  \
    mzswap((yield_co), (yield_co)->main_co);    \
} while(0)

#define mz_yield() do { \
    mz_yield1(mz_gtls_co);  \
} while(0)

#define mz_get_arg() (mz_gtls_co->arg)

#define mz_get_co() ({(void)0; mz_gtls_co;})

#define mz_co() ({(void)0; mz_gtls_co;})

extern void mz_destory(mz_t* co);

#define mz_is_main_co(co) ({((co)->main_co) == NULL;})

#define mz_exit1(co) do { \
    (co)->is_end = 1; \
    mz_assert((co)->share_stack->owner == (co)); \
    (co)->share_stack->owner = NULL; \
    (co)->share_stack->align_validsz = 0; \
    mz_yield1((co));    \
    mz_assert(0);       \
} while (0)

#define mz_exit() do { \
    mz_exit1(mz_gtls_co); \
} while (0)

}

#endif


