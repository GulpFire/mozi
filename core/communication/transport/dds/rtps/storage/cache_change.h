#ifndef MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_STORAGE_CACHE_CHANGE_H
#define MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_STORAGE_CACHE_CHANGE_H

#include <mozi/core/time/time.h>

#include <atomic>
#include <cassert>

namespace mozi {
namespace core {
namespace communication {
namespace dds {

enum ChangeKind
{
    ALIVE,
    NOT_ALIVE_DISPOSED,
    NOT_ALIVE_UNREGISTERED,
    NOT_ALIVE_DISPOSED_UNREGISTERED
};

struct CacheChangeWriterInfo
{
    size_t num_size_submessage = 0;
    CacheChange* volatile prev = nullptr;
    CacheChange* volatile next = nullptr;
		std::atomic_bool is_linked{false};
};

struct CacheChangeReaderInfo
{
    Time reception_timestamp;
    int32_t disposed_generation_count;
    int32_t no_writers_generation_count;
		uint32_t writer_ownership_strength;
};

struct CacheChange
{
	ChangeKind kind = ALIVE;
	GUID writerGUID{};
};

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi

#endif
