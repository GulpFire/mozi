#ifndef MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_STORAGE_HISTORY_H
#define MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_STORAGE_HISTORY_H

#include <mozi/core/communciation/dds/rtps/storage/change_pool.h>
#include <mozi/core/communication/dds/rtps/storage/history_attributes.h>
#include <mozi/core/communication/dds/rtps/storage/payload_pool.h>
#include <mozi/core/communication/dds/rtps/common/guid.h>

#include <mozi/core/common/data_structure/sequence_number.h>
#include <mozi/core/utils/timed_mutex.h>

#include <cassert>
#include <functional>
#include <memory>
#include <mutex>

namespace mozi {
namespace core {
namespace communication {
namespace dds {

class History
{
    protected:
        History(const HistoryAtrributes& att);
        Hitsory(History&&) = delete;
        History& operator=(History&&) = delete;
        virtual ~History();

    public:
        using CacheChangeIterator = std::vector<CacheChange*>::iterator;
        using ReverseCacheChangeiterator = std::vector<CacheChange*>::reverse_iterator;
        using ConstCacheChangeIterator = std::vector<CacgeChange*>::const_iterator;
        using CalculateSizeFunc = std::function<uint32_t()>;

        HistoryAttributes attr_;

        inline bool reverseCache(CacheChange** change, 
                const CalculateSizeFunc& func)
        {
            return doReverseCache(change, func());
        }

        inline bool reverseCache(CacheChange** change, uint32_t data_size)
        {
            return doReverseCache(change, data_size);
        }

        inline bool reverseCache(CacheChange* change)
        {
            return doReverseCache(change);
        }

        bool isFull() { return is_history_full_; }

        size_t getHistorySize() 
        {
            std::lock_guard<RecursiveTimedMutex> guard(*mp_mtx_);
            return changes_.size();
        }

        ConstCacheChangeIterator findChangeNts(CacheChange* ch);

        virtual CacheChangeIterator removeChangeNts(
                ConstCacheChangeIterator removal, bool release = true);
        
        bool removeAllChanges();

        bool removeChanges(CacheChange* change);

        ConstCacheChangeIterator findChange(CacheChange* change)
        {
            std::lock_guard<RecursiveTimedMutex> guard(*mp_mtx_);
            return findChangeNts(change);
        }

        virtual bool matchesChange(const CacheChange* inner_change,
                CacheChange* outer_change);

        CacheChangeIterator removeChange(ConstCacheChangeIterator removal,
                bool release = true)
        {
            std::lock_guard<RecursiveTimedMutex> guard(*mp_mtx_);
            return removeChangeNts(removal, release);
        }

        CacheChangeIterator changesBegin() { return changes_.begin(); }

        ReverseCacheChangeIterator changesRbegin() { return changes_.rbegin();}
        CacheChangeIterator changesEnd() { return changes_.end(); }

        ReverseCacheChangeIterator changesRend() { return changes_.rend(); }

        bool getMinChange(CacheChange** min_change);

        bool getMaxChange(CacheChange** max_change);

        inline uint32_t getTypeMaxSerialized() { return attr_.payload_max_size; }

        inline RecursiveTimedMutex* getMutex()
        {
            assert(mp_mtx_ != nullptr);
            return mp_mtx_;
        }

        bool getChange(const SequenceNumber& sn, const GUID& guid,
                CacheChange** change) const;

        ConstChangeCacheIterator getChangeNts(const SequenceNumber& sn,
                const GUID& guid, CacheChange** change,
                ConstCacheChangeIterator hint) const;

        bool getEarliestChange(CacheChange** change);

    protected:
        std::vector<CacheChange*> changes_;
        bool is_history_full_{false};
        RecursiveTimedMutex* mp_mtx{nullptr};
        void printChangesSeqNum2();
        virtual bool doReserveCache(CacheChange** change, uint32_t size) = 0;
        virtual void doReleaseCache(CacheChange* change) = 0;
};

} // namespace dds
} // namespace communication
} // namespace core
}// namespace mozi

#endif
