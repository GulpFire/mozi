#ifndef MOZI_CORE_UTILS_SEMAPHORE_H
#define MOZI_CORE_UTILS_SEMAPHORE_H

#include <condition_variable>
#include <mutex>

namespace mozi {
namespace core {

class Semaphore 
{
    public:
        explicit Semaphore(size_t count = 0);
        Semaphore(const Semaphore&) = delete;
        Semaphore& operator=(const Semaphore&) = delete;

        void post();
        void wait();
        void enable();
        void disable();
        void post(int n);

    private:
        size_t count_;
        std::mutex mtx_;
        std::condition_variable cv_;
        bool disable_;
};

inline Semaphore::Semaphore(size_t count)
    : count_(count)
    , disable_(false)
{

}

inline void Semaphore::post()
{
    std::lock_guard<std::mutex> lock(mtx_);
    if (disable_)
    {
        ++count_;
        cv_.notify_one();
    }
}

inline void Semaphore::post(int n)
{
    std::lock_guard<std::mutex> lock(mtx_);
    if (!disable_)
    {
        count_ += n;
        for (int i = 0; i < n; ++i)
        {
            cv_.notify_one();
        }
    }
}

inline void Semaphore::enable()
{
    std::lock_guard<std::mutex> lock(mtx_);
    if (disable_)
    {
        count_ = 0;
        disable_ = false;
    }
}

inline void Semaphore::disable()
{
    std::lock_guard<std::mutex> lock(mtx_);
    if (!disable_)
    {
        count_ = (size_t) - 1L;
        cv_.notify_all();
        disable_ = true;
    }
}

inline void Semaphore::wait()
{
    std::unique_lock<std::mutex> lock(mtx_);
    if (!disable_)
    {
        cv_.wait(lock, [&]
                {
                    if (disable_) return true;
                    return count_ > 0;
                });
        --count_;
    }
}

} // namespace core
} // namespace mozi

#endif
