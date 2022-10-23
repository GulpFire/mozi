#ifndef MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_STORAGE_HISTORY_ATTRIBUTES_H
#define MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_STORAGE_HISTORY_ATTRIBUTES_H

#include <mozi/core/communication/transport/dds/rtps/resource/resource_management.h>

#include <cstdint>

namespace mozi {
namespace core {
namespace communication {
namespace dds {

class HistoryAttributes
{
    public:
        MemoryManagementPolicy memory_policy;
        uint32_t payload_max_size;
        int32_t initial_reserved_caches;
        int32_t maximum_reserved_caches;
        int32_t extra_reserved_caches;

        HistoryAttributes()
            : memory_policy(PREALLOCATED_MEMORY_MODE)
            , payload_max_size(500)
            , initial_reserved_caches(500)
            , maximum_reserved_caches(0)
            , extra_reserved_caches(1)
        {
        
        }

        HistoryAttributes(MemoryManagementPolicy mem_policy, 
                uint32_t payload, int32_t initial, int32_t max_res)
            : memory_policy(mem_policy)
            , payload_max_size(payload)
            , initial_reserved_caches(initial)
            , maximum_reserved_caches(max_res)
            , extra_reserved_caches(1)
        {
            
        }

        HitsoryAttributes(MemoryManagementPolicy mem_policy,
                uint32_t payload, int32_t initial, 
                int32_t max_res, int32_t extra)
            : memory_policy(mem_policy)
            , payload_max_size(payload)
            , initial_reserved_caches(initial)
            , maximum_reserved_caches(max_res)
            , extra_reserved_caches(extra)
        {
        
        }

        virtual ~HitsoryAttributes()
        {
        
        }
};

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi

#endif
