#ifndef MOZI_CORE_COMMUNICATION_DDS_RTPS_STORAGE_READER_HISTORY_H
#define MOZI_CORE_COMMUNICATION_DDS_RTPS_STORAGE_READER_HISTORY_H

#include <mozi/core/communication/transport/dds/rtps/storage/history.h>
#include <mozi/core/communication/transport/dds/rtps/storage/cache_change.h>

/**
 * Each write operation on a DDS
 * DataWriter adds a CacheChange to the HistoryCache of its corresponding RTPS Writer. The RTPS Writer subsequently transfers the CacheChange to the HistoryCache of all matching RTPS Readers. On the receiving

side, the DDS DataReader is notified by the RTPS Reader that a new CacheChange has arrived in the

HistoryCache, at which point the DDS DataReader may choose to access it using the DDS read or take API.
*/

namespace mozi {
namespace core {
namespace communication {
namespace dds {
class WriterProxy;
class RTPSReader;

class ReaderHistory : public History
{
    friend class RTPSReader;

    ReaderHistory(ReaderHistory&&) = delete;
    ReaderHistory& operator=(ReaderHistory&&) = delete;

    public:
    ReaderHistory(const HistoryAttributes& attr);
    ~ReaderHistory() override;

    virtual bool canChangeBeAddedNts(const GUID& writer_guid, 
            uint32_t total_payload_size, size_t unknown_missing_changes_up_to,
            bool& will_never_be_accepted) const;

    virtual bool received_change(CacheChange* change,
            size_t unknown_missing_changes_up_to);

    virtual bool completed_change(CacheChange* change)
    {
        (void)change;
        return true;
    }

    bool add_change(CacheChange* change);

    CacheChangeIterator removeChangeNts(ConstCacheChangeIterator removal,
            bool release = true) override;
    
    bool matchesChange(const CacheChange* inner, CacheChange* outer) override;
    
    using History::remove_change;
    
    bool removeChangesWithGuid(const GUID& guid);
    
    bool removeFragmentedChangesUntil(const SequenceNumber& sn,
            const GUID& writer_guid);
    
    bool getMinChangeFrom(CacheChange** min_change, const GUID& writer_guid);
    
    virtual void writerUnmatched(const GUID& writer_guid,
            const SequenceNumber& last_notified_sn);
    
    protected:
    bool doReserveCache(CacheChange** change, uint32_t size) override;
    
    void doReleaseCache(CacheChange* change) override;

    template <typename Pred>
    inline void removeChangesWithPred(Pred pred)
    {
        assert(nullptr != mp_reader_);
        assert(nullptr != mp_mtx_);
        
        std::lock_guard<RecursiveTimedMutex> guard(*mp_mtx_);
        std::vector<CacheChange*>::iterator new_end = 
            std::remove_if(changes_.begin(), changes_.end(), pred);
        while (new_end != changes_.end())
        {
            new_end = removeChangeNts(new_end);
        }
    }
    
    RTPSReader* mp_reader_;

};
} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi

#endif
