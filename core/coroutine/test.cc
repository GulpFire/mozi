#include "mz_coroutine.h"
#include <chrono>
#include <iostream>

using namespace mozi::core::coroutine;

#define TOTAL 100000

int ping_cnt = 0;
int pang_cnt = 0;

void pang(MzCoroutine& co) noexcept
{
    ++pang_cnt;
    co.Yield();
}

void ping() noexcept
{
    MzCoroutine co(pang);
    ++ping_cnt;
    CoState state = co.Resume();
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    while (pang_cnt < TOTAL) {
    ping();
    }
    auto end = std::chrono::high_resolution_clock::now();
    long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::printf("%lldms: pingCount %d, pangCount %d\n", duration, ping_cnt, pang_cnt);
}
