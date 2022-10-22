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
};

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi

#endif
