#ifndef MOZI_CORE_COMMON_DATA_STRUCTURE_DOUBLE_BUFFERED_QUEUE_H
#define MOZI_CORE_COMMON_DATA_STRUCTURE_DOUBLE_BUFFERED_QUEUE_H

#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>

namespace mozi {
namespace core {
template <typename T>
class DoubleBufferedQueue
{
    public:
        DoubleBufferedQueue()
            : front_queue_(&queue_a_)
            , back_queue_(&queue_b_)
        {}

        void Swap()
        {
            std::unique_lock<std::mutex> front_gurad(front_queue_mtx_);
            std::unique_lock<std::mutex> back_guard(back_queue_mtx_);
            std::queue<T>().swap(*front_queue_);
            auto* swap = back_queue_;
            back_queue_ = front_queue_;
            front_queue_ = swap;
        }

        void Push(const T& item)
        {
            std::unique_lock<std::mutex> guard(back_queue_mtx_);
            back_queue_->push(item);
        }

        T& Front()
        {
            std::unique_lock<std::mutex> guard(front_queue_mtx_);
            return front_queue_->front();
        }

        const T& Front() const
        {
            std::unique_lock<std::mutex> guard(front_queue_mtx_);
            return front_queue_mtx_->front();
        }

        void Pop()
        {
            std::unique_lock<std::mutex> guard(front_queue_mtx_);
            front_queue_->pop();
        }

        bool Empty() const
        {
            std::unique_lock<std::mutex> guard(front_queue_mtx_);
            return front_queue_->empty();
        }

        bool BothEmpty() const
        {
            std::unique_lock<std::mutex> front_guard(front_queue_mtx_);
            std::unique_lock<std::mutex> back_guard(back_queue_mtx_);
            return front_queue_mtx_->empty() && back_queue_mtx_->empty();
        }

        size_t Size() const
        {
            std::unique_lock<std::mutex> guard(front_queue_mtx_);
            return front_queue_->size();
        }

        void Clear()
        {
            std::unique_lock<std::mutex> front_guard(front_queue_mtx_);
            std::unique_lock<std::mutex> back_guard(back_queue_mtx_);
            std::queue<T>().swap(*front_queue_);
            std::queue<T>().swap(*back_queue_);
        }

    private:
        std::queue<T> queue_a_;
        std::queue<T> queue_b_;
        std::queue<T>* front_queue_;
        std::queue<T>* back_queue_;

        mutable std::mutex front_queue_mtx_;
        mutable std::mutex back_queue_mtx_;
};
} // namespace
} // namespace mozi

#endif
