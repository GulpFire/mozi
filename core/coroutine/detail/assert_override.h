#ifndef MOZI_CORE_COROUTINE_DETAIL_ASSERT_OVERRIDE_H
#define MOZI_CORE_COROUTINE_DETAIL_ASSERT_OVERRIDE_H

#include "mzco.h"

extern "C" {
#define likely(x)       mz_likely(x)
#define unlikely(x)     mz_unlikely(x)
#define assert(EX)      mz_assert(EX)
#define assertptr(ptr)  mz_assertptr(ptr)
#define assertalloc_bool(b) mz_assertalloc_bool(b)
#define assertalloc_ptr(ptr) mz_assertalloc_ptr(ptr)
}

#endif
