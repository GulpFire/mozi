#include <coro/coro.hpp>
#include <iostream>
#include <chrono>

const int TOTAL = 1000000;
int ping_cnt = 0;
int pang_cnt = 0;

int main()
{
    auto ping = []() -> coro::task<>
    {
        ping_cnt++;
        co_return;
    };

    auto pang = [&]() -> coro::task<> 
    {
        pang_cnt++;
        co_await ping();
    };

    auto start = std::chrono::high_resolution_clock::now();
    while (pang_cnt < TOTAL)
    {
        coro::sync_wait(pang());
    }

    auto end = std::chrono::high_resolution_clock::now();
    long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::printf("%lldms: ping count: %d, pang count: %d\n", duration, ping_cnt, pang_cnt);
}
