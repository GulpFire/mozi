#ifndef MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_RESOURCE_RESOURCE_MANAGEMENT_H
#define MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_RESOURCE_RESOURCE_MANAGEMENT_H

namespace mozi {
namespace core {
namespace communication {
namespace dds {

typedef enum MemoryManagementPolicy
{
    PREALLOCATED_MEMORY_MODE,
    PREALLOCATED_WITH_REALLOC_MEMORY_MODE,
    DYNAMIC_RESERVE_MEMORY_MODE,
    DYNAMIC_REUSABLE_MEMORY_MODE
} MemoryManagementPolicy;

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi

#endif
