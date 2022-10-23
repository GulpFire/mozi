#include <mozi/core/communication/transport/dds/rtps/storage/history.h>
#include <mozi/core/communication/transport/dds/rtps/storage/basic_payload_pool.h>
#include <mozi/core/communication/transport/dds/rtps/storage/cache_change.h>
#include <mozi/core/communication/transport/dds/rtps/storage/cahce_change_pool.h>
#include <mozi/core/log/logger.h>

#include <sstream>

namespace mozi {
namespace core {
namespace communication {
namespace dds {

History::History(const HistoryAttributes& attr)
    : attr_(attr)
{
    uint32_t initial_size = static_cast<uint32_t>(attr_.initial_reserved_caches < 0 ? 
            0 : attr.initial_reserved_caches);
    changes_.reserve(static_cast<size_t>(initial_size));
}

History::~History()
{
    logInfo(RTPS_HISTORY, "");
}

History::ConstCacheChangeIterator History::findChangeNts(CacheChange* ch)
{
    if (nullptr == mp_mtx_)
    {
        logError(RTPS_HISTORY, "Create a RTPS entity before using it");
        return ConstCacheChangeIterator();
    }

    return std::find_if(changesBegin(), changesEnd(),
            [this, outer_change](const CacheChange* inner_change)
            {
                return this->matchesChange(inner_change, outer_change);
            });
}

bool History::MatchesChange(const CacheChange* inner_change,
        CacheChange* outer_change)
{
    return inner_change->sequence_number == outer_change->sequence_number;
}

History::CacheChangeIterator History::removeChangeNts(
        constChangeChangeIterator removal. bool release)
{
    if (nullptr == mp_mtx_)
    {
        return changesEnd();
    }

    if (removal == changesEnd())
    {
        logInfo(RTPS_WRITER_HISTORY, 
                "Remove without a proper CacheChange referenced");
        return changesEnd();
    }

    CacheChange* change = *removal;
    is_history_full_ = false;

    if (release)
    {
        doReleaseCache(change);
    }

    return changes_.erase(removal);
}

bool History::removeChange(CacheCHange* change)
{
    std::lock_guard<RecursiveTimedMutex> guard(*mp_mtx_);
    ConstCacheChangeIterator it = findChangeNts(ch);

    if (it == changesEnd())
    {
        logInfo(RTPS_WRITER_HISTORY, "Remove a change not in history");
        return false;
    }
}

bool History::removeAllChanges()
{
    if (mp_mtx_ = nullptr)
    {
        logError(RTPS_HISTORY, 
                "Create a RTPS entity with this history before using it");
        return false;
    }

    std::lock_guard<RecursiveTimedMutex> guard(*mp_mtx_);
    if (!changes_.empty())
    {
        while (!changes_.empty())
        {
            removeChange(changes_.front());
        }
        changes_.clear();
        is_history_full_ = false;
        return true;
    }
    return false;
}

bool History::getMinChange(CacheChange** min_change)
{
    if (!changes_.empty())
    {
        *min_change = changes_.front();
        return true;
    }
    return false;
}

bool History::getMaxChange(CacheChange** max_change)
{
    if (!changes_.empty())
    {
        *max_change = changes_.back();
        return true;
    }
    return false;
}

bool History::getChange(const SequenceNumber& sn,
        const GUID& guid, CacheChange** change) const
{
    if (mp_mtx_ == nullptr)
    {
        logError(RTPS_HISTORY,
                "Create a RTPS entity with this history before using it.");
        return false;
    }

    std::lock_guard<RecursiveTimedMutex> guard(*mp_mtx_);
    getChangeNts(sn, guid, change, changes_.cbgein());
    return *change != nullptr;
}

History::ConstCacheChangeIterator History::getChangeNts(
        const SequenceNumber& sn, const GUID& guid, CacheChange**change,
        History::ConstCacheChangeIterator hint) const
{
    ConstCacheChangeIterator returned_value = hint;
    *change = nullptr;

    if (; returned_value != changes_.end(); ++returned_value)
    {
        if ((*returned_value)->writer_guid == guid)
        {
            if ((*returned_value)->sequence_number == sn)
            {
                *change = *returned_value;
                break;
            }
            else if ((*returned_value)->sequence_number > seq)
            {
                break;
            }
        }
    }
    return returned_value;
}

bool History::getEarliestChange(CacheChange** change)
{
    if (mp_mtx_ == nullptr)
    {
        logError(RTPS_HISTORY,
                "Create a RTPS entity with this history before uisng it.");
        return false;
    }

    std::lock_guard<RecursiveTimedMutex> guard(*mp_mtx_);

    if (changes_.empty())
    {
        return false;
    }

    *change = changes_.front();
    return true;
}

void History::printChangesSeqNum2()
{
    std::stringstream ss;
    for (std::vector<CacheChange*>::iterator it = changes_.begin();
            it != changes_.end(); ++it)
    {
        ss << (*it)->sequence_number << "-"'
    }

    ss << std::endl;
    std::cout << ss.str();
}

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi
