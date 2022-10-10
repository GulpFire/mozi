#include "mzco.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "assert_override.h"

#include <iostream>

void co_fp0()
{
    int* iretp = (int*)mz_get_arg();
    mz_t* this_co = mz_get_co();
    int ct = 0;
    while (ct < 6)
    {
        printf("co:%p save_stack:%p share_stack:%p yieild_ct:%d\n",
                this_co, this_co->save_stack.ptr,
                this_co->share_stack->ptr, ct);
        mz_yield();
        (*iretp)++;
        ct++;
    }
    printf("co:%p save_stack:%p share_stack:%p co_exit()\n",
            this_co, this_co->save_stack.ptr,
            this_co->share_stack->ptr);
    mz_exit();
}

int main()
{
#ifdef MZ_USE_VALGRIND
    if (0)
    {
        printf("%s does not have valgrind test yet,"
                "so bypass this test right now.\n", __FILE__);
        exit(0);
    }
#endif
    mz_thread_init(NULL);

    mz_t* main_co = mz_create(NULL, NULL, 0, NULL, NULL);
    mz_share_stack_t* sstk = mz_share_stack_new(0);
    int mz_ct_arg_point_to_me = 0;
    mz_t* co = mz_create(main_co, sstk, 0, co_fp0, &mz_ct_arg_point_to_me);

    int ct = 0;
    while (ct < 6)
    {
        assert(co->is_end == 0);
        mz_resume(co);
        assert(mz_ct_arg_point_to_me == ct);
        printf("main_co:%p\n", main_co);
        ct++;
    }
    mz_resume(co);
    assert(mz_ct_arg_point_to_me == ct);
    assert(co->is_end);
    printf("main_co:%p\n", main_co);
    mz_destory(co);
    co = NULL;
    mz_share_stack_destory(sstk);
    sstk = NULL;
    mz_destory(main_co);
    main_co = NULL;

    return 0;
}
