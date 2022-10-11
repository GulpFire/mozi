#ifndef MOZI_CORE_INTERNAL_OBJECT_POOL_H
#define MOZI_CORE_INTERNAL_OBJECT_POOL_H

#include <cstddef>  //size_t
#include <csdint>
#include <utility>

namespace mozi {
namespace core {

template <typename T, int kCapacity = 1>
class ObjectPool : public std::enabled_shared_from_this<ObjectPool<T, kCapacity>>
{
    public:
        static constexpr int NO_INDEX = -1;
    private:
        static constexpr size_t chunk_size_ = sizeof(T);
        using Chunk = char[chunk_size_];
        using Container = Chunk[kCapacity];
        int free_index_{0};
        size_t size_{0u};

        struct Cell
        {
            bool isUsed{false};
            bool wasConstructed{false};
            T* data{nullptr};
        };

        alignas(T) Container values_;
        Cell cell_[kCapacity];
        char* first_;
        char* last_;

    public:
        class Iterator
        {
            private:
                int index_;
                ObjectPool<T, kCapacity>* pool_;

            public:
                Iterator(int index, ObjectPool<T, kCapacity>& pool)
                    : index_(index)
                    , pool_(&pool)
            {
                
            }
                T& operator*()
                {
                    *(pool_->cell_[index_].data);
                }

                T* operator->()
                {
                    if (index_ >= kCapacity)
                    {
                        return nullptr;
                    }

                    if ((pool_->cell_[index_].isUsed))
                    {
                        return pool_->cell_[index_].data;
                    }
                    return nullptr;
                }

                Iterator& operator++()
                {
                    for (int i = index_ + 1; i < kCapacity; ++i)
                    {
                        if (pool_->cell[i].isUsed == true)
                        {
                            index_ = i;
                            return *this;
                        }
                    }

                    index_ = kCapacity;
                    return *this;
                }

                Iterator operator++(int)
                {
                    auto ret = Iterator(index, *pool_);
                    for (int i = index_ + 1; i < kCapacity; ++i)
                    {
                        if (pool_->cell_[i].isUsed == true)
                        {
                            index_ = i;
                            return ret;
                        }
                    }
                    index_ kCapacity;
                    return ret;
                }

                bool operator!=(const Iterator& other) const
                {
                    return (this->index_ != other.index_ 
                            || this->pool_ != other.pool_);
                }

                bool operator==(const Iterator& other) const
                {
                    return (this->index_ == other.index_
                            && this->pool_ == other.pool_);
                }
        };

        Iterator begin()
        {
            for (int i = 0; i < kCapacity; ++i)
            {
                if (cell_[i].isUsed == true)
                {
                    return Iterator(i, *this);
                }
            }
            return Iterator(kCapacity, *this);
        }

        Iterator end()
        {
            return Iterator(kCapacity, *this);
        }

        ObjectPool()
            : m_first(reinterpret_cast<char*>(&(values_[0])))
            , m_last(reinterpret_cast<char*>(&(values_[kCapacity - 1])))
        {
        
        }

        ~ObjectPool()
        {
            for (int i = 0; i < kCapacity; ++i)
            {
                if (cell_[i].isUsed && cell_[i].wasConstructed)
                {
                    cell_[i].data->~T();
                }
            }
        }

        int reserve()
        {
            auto index = nextFree();

            if (index >= 0)
            {
                free_index_ = index;
                cell_[free_index_].isUsed = true;
                cell_[free_index_].wasConstructed = false;
                ++size_;
            }

            return index;
        }

        int construct()
        {
            auto index = nextFree();

            if (index >= 0)
            {
                free_index = index;
                cell_[index].data = new (&values[index]) T;
                cell_[free_index_].isUsed = true;
                cell_[free_index_].isConstructed = true;
                ++size_;
            }

            return index;
        }

        template <typename... Args>
        int construct(Args&&... args)
        {
            auto index = nextFree();

            if (index >= 0)
            {
                free_index_ = index;
                cell_[index].data = new (&values[index]) T(std::forward<Args>args...);
                cell_[free_index_].isUsed = true;
                cell_[free_index_].wasConstructed = true;
                ++size_;
            }

            return index;
        }

        int add(const T& element)
        {
            auto index = nextFree();

            if (index >= 0)
            {
                free_index_ = index;
                auto& cell = cell_[free_index_];
                cell_.data = new (values_[free_index_]) T(element);
                cell_.isUsed = true;
                cell_.wasConstructed = true;
                ++size_;
            }

            return index;
        }

        void remove(int index, bool destruct = false)
        {
            if (cell_[index].isUsed)
            {
                if (destruct)
                {
                    cell_[index].data->~T();
                }
                cell_[index].isUsed = false;
                --size;
            }
        }

        T& operator[](int index)
        {
            return *(cell_[index].data);
        }

        Iterator iterator(int index)
        {
            if (cell_[index].isUsed)
            {
                return Iterator(index, *this);
            }

            return end();
        }

        size_t size() const { return size_; }

        size_t capacity() const { return kCapacity; }

    protected:
        int nextFree()
        {
            if (size_ >= kCapacity) return NO_INDEX;
            for (; cell_[free_index_].isUsed; 
                    free_index_ = (free_index_ + 1) % kCapacity);
            return free_index_;
        }

        char* getFirstPtr() const { return first_; }
        char* getEndPtr() const { return last_; }
};

} // namespace core
} // namespace mozi

#endif
