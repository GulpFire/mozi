#ifndef MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_STORAGE_PAYLOAD_POOL_H
#define MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_STORAGE_PAYLOAD_POOL_H

#include <mozi/core/communication/transport/dds/rtps/common/serialized_payload.h>

#include <cstdint>
#include <memory>

struct CacheChange;

class PayloadPool
{
    public:
        virtual ~PayloadPool() = default;
        virtual bool getPayload(uint32_t size, CacheChange& change) = 0;
        virtual bool getPayload(SerializedPayload& data, 
                PayloadPool*& data_owner, CacheChange& cache_change) = 0;
        virtual bool releasePayload(CacheChange& cache_change) = 0;
};

#endif
