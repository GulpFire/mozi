#include "../mzco.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "../assert_override.h"

using namespace mozi::core::coroutine;

void foo(int ct)
{
    printf("co:%p save_stack:%p share_stack:%p yield_ct:%d\n",
            mz_get_co(), mz_get_co()->save_stack.ptr, 
            mz_get_co()->share_stack->ptr, ct);
    mz_yield();
    
    (*((int*)(mz_get_arg())))++;
}

void co_fp0()
{
    mz_t* this_co = mz_get_co();
    assert(!mz_is_main_co(this_co));
    assert(this_co->fp==(void*)co_fp0);
    assert(this_co->is_end==0);
    int ct = 0;

    while (ct < 6)
    {
        foo(ct);
        ct++;
    }

    printf("co:%p save_stack:%p share_stack:%p co_exit()\n",
            this_co, this_co->save_stack.ptr,
            this_co->share_stack->ptr);
    mz_exit();
    assert(0);
}

int main()
{
    mz_thread_init(NULL);
    mz_t* main_co = mz_create(NULL, NULL, 0, NULL, NULL);
    assertptr(main_co);

    mz_share_stack_t* sstk = mz_share_stack_new(0);
    assertptr(sstk);
    mz_share_stack_t* sstk2 = mz_share_stack_new(0);
    assertptr(sstk2);

    int co_ct_arg_point_to_me = 0;
    int co2_ct_arg_point_to_me = 0;
    int co3_ct_arg_point_to_me = 0;

    mz_t* co = mz_create(main_co, sstk, 0, co_fp0, &co_ct_arg_point_to_me);
    assertptr(co);
    mz_t* co2 = mz_create(main_co, sstk2, 0, co_fp0, &co2_ct_arg_point_to_me);
    assertptr(co2);
    mz_t* co3 = mz_create(main_co, sstk2, 0, co_fp0, &co3_ct_arg_point_to_me);
    assertptr(co3);

    int ct = 0;
    while (ct < 6)
    {
        assert(co->is_end == 0);
        mz_resume(co);
        assert(co_ct_arg_point_to_me == ct);

        assert(co2->is_end == 0);
        mz_resume(co2);
        assert(co2_ct_arg_point_to_me == ct);

        assert(co3->is_end == 0);
        mz_resume(co3);
        assert(co3_ct_arg_point_to_me == ct);

        printf("main_co:%p\n", main_co);
        ct++;
    }

    mz_resume(co);
    assert(co_ct_arg_point_to_me == ct);
    assert(co->is_end);

    mz_resume(co2);
    assert(co2_ct_arg_point_to_me == ct);
    assert(co2->is_end);

    mz_resume(co3);
    assert(co3_ct_arg_point_to_me == ct);
    assert(co3->is_end);

    printf("main_co:%p\n", main_co);

    mz_destory(co);
    co = NULL;
    mz_destory(co2);
    co2 = NULL;
    mz_destory(co3);
    co3 = NULL;

    mz_share_stack_destory(sstk);
    sstk = NULL;
    mz_share_stack_destory(sstk2);
    sstk2 = NULL;

    mz_destory(main_co);
    main_co = NULL;

    return 0;
}
