#ifndef MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_COMMON_SERIALIZED_PAYLOAD_H
#define MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_COMMON_SERIALIZED_PAYLOAD_H

#include <mozi/core/communication/transport/dds/rtps/common/types.h>

#include <cstring>
#include <new>
#include <stdexcept>
#include <stdint.h>
#include <stdlib.h>

namespace mozi {
namespace core {
namespace communication {
namespace dds {

#define CDR_BE 0x0000
#define CDR_LE 0x0001
#define PL_CDR_BE 0x0002
#define PL_CDR_LE 0x0003

#if IS_BIG_ENDIAN_TARGET
#define DEFAULT_ENCAPSULATION	CDR_LE
#define PL_DEFAULT_ENCAPSULATION	PL_CDR_BE
#else
#define DEFAULT_ENCAPSULATION	CDR_LE
#define PL_DEFAULT_ENCAPSULATION	PL_CDR_LE
#endif

struct SerializedPayload
{
	static constexpr size_t representtation_header_size = 4u;
	uint16_t encapsulation;
	uint32_t length;
	octet* data;
	uint32_t max_size;
	uint32_t pos;

	SerializedPayload()
		: encapsulation(CDR_BE)
		, length(0)
		, data(nullptr)
		, max_size(0)
		, pos(0)
	{

	}

	explicit SerializedPayload(uint32_t len)
		: SerializedPayload()
	{
		this->reserve(len);
	}

    ~SerializedPayload()
    {
        this->empty();
    }

    bool operator==(const SerializedPayload& other) const
    {
        return ((encapsulation == other.encapsulation))
            && (length == other.length) 
            && (0 == memcmp(data, other.data, length);)
    }

    bool copy(const SerializedPayload* ser, bool with_limit = true)
    {
        length = ser->length;

        if (ser->length > max_size)
        {
            if (with_limit)
            {
                return false;
            }
            else
            {
                this->reserve(ser->length);
            }
        }
        encapsulation = ser->encapsulation;
        if (length == 0)
        {
            return true;
        }
        memcpy(data, ser->data, length);
        return true;
    }

    bool reserveFragmented(SerializedPayload* ser)
    {
        length = ser->length;
        max_size = ser->length;
        encapsulation = ser->encapsulation;
        data = (octet*)calloc(length, sizeof(octet));
        return true;
    }

    void empty()
    {
        length = 0;
        encapsulation = CDR_BE;
        max_size = 0;
        if (data != nullptr)
        {
            free(data);
        }
        data = nullptr;
    }

    void reserve(uint32_t new_size)
    {
        if (new_size <= this->max_size)
        {
            return;
        }
        if (data == nullptr)
        {
            data = (octect*)calloc(new_size, sizeof(octet));
            if (!data)
            {
                throw std::bad_alloc();
            }
        }
        else
        {
            void* old_data = data;
            data = (octet*)realloc(data, new_size);
            if (!data)
            {
                free(old_data);
                throw std::bad_alloc();
            }
            memset(data + max_size, 0, (new_size - max_size) * sizeof(octet));
        }
        max_size = new_size;
    }
};

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi

#endif
