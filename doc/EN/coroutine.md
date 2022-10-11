# Coroutine

## Introduction
## Data Structure
Structure `mz_save_stack_t` 

|Name|Class|Description|
|-|-|-|
|`ptr`|void\*||
|`sz`|size\_t||
|`valid_sz`|size\_t||
|`max_cpsz`|size\_t||
|`ct_save`|size\_t||
|`ct_restore`|size\_t||

Structure `mz_s` aliased as `mz_t`
|Name|Class|Description|
|-|-|-|
|`reg`|void\*|use to save context|
|`main_co`|mz\_t\*|main coroutine|
|`arg`|void\*||
|`is_end`|char||
|`fp`|mz\_cofuncp\_t|function handler|
|`save_stack`|mz\_save\_stack\_t||
|`share_stack`|mz\_share\_stack\_t\*||


Structure `mz_share_stack_t`

|Name|Class|Description|
|-|-|-|
|`ptr`|void\*||
|`sz`|size\_t||
|`align_highptr`|void\*||
|`align_retptr`|void\*||
|`align_validsz`|size\_t||
|`align_limit`|size\_t||
|`owner`|mz\_t||
|`guard_page_enabled`|char||
|`real_ptr`|void\*||
|`real_sz`|size\_t||
|`valgrind_stk_id`|unsigned long||

## API
|Name|Return|Description|
|-|-|-|
|`mz_thread_init(mz_cofuncp_t last_word_co_fp)`|void|Initialize the coroutine enviroment in current thread. It will  store the current control words of FPU and MXCSR into a thread-local global variable. When parameter is not NULL, it will subsititute the default protector to do some last words stuff.|
|`mz_swap(mz_t* from_co, mz_t* to_co)`|void||
|`mz_share_stack_new(size_t sz)`|mz\_share\_stack\_t\*|Equal to `mz_share_stack_new2(sz, 1)`|
|`mz_share_stack_new2(size_t sz, char guard_page_enabled)`|mz\_share\_stack\_t|Creates a new share stack with a advisory memory size of sz in bytes and may have a guard page (read-only) for the detection of stack overflow which is depending on the 2nd argument `guard_page_enabled.`|
|`mz_share_stack_destory(mz_share_stack_t* sstk)`|void|Destory the share stack `sstk`|
|`mz_create(mz_t* main_co, mz_share_stack_t* share_stack, size_t save_stack_sz, mz_cofuncp_f fp, void* arg)`|mz\_t\*|Create a new coroutine. Main coroutine's share stack is the default thread stack, so it should be created before all the other non-main coroutine in the thread. The first argument `main_co` denotes the main coroutine that current coroutine will `mz_yield` to in the future context switching. For non-main coroutine, this argument must not be NULL. The second argument `share_stack` is the address of a share stack that the non-main coroutine will use as its executing stack in the future. It also must not be NULL for non-main coroutine. The third parameter `save_stack_sz` specifies the init size of the private save stack of this coroutine. The forth parameter `fp` is the entry function pointer that must not be NULL. The last argument `arg` will set to `co->arg` of the coroutine. This function will return a coroutine in `Init` state for non-main coroutine.|
|`mz_resume(mz_t* co)`|void|Yield from the caller main coroutine to start or continue the execution of co|
|`mz_yield`|void|Yield the execution of the caller coroutine and resume `co->main_co`|
|`mz_get_co`|mz\_t\*|Return the pointer of the current non-main coroutine. The caller must be another non-main coroutine|
|`mz_exit`|void|Do the same as `mz_yield` but set `co->is_end` to 1, marking the co at the status of `end`|
|`mz_destory(mz_t* co)`|void|Destory the `co`, and its private save stack, if it is not main coroutine.|
