#ifndef MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_COMMON_GUID_H
#define MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_COMMON_GUID_H

#include <mozi/core/communication/transport/dds/rtps/common/types.h>

#include <cstdint>
#include <cstring>
#include <sstream>
#include <iomanip>

namespace mozi {
namespace core {
namespace communication {
namespace dds {

struct GuidPrefix
{
	static constexpr unsigned int size = 12;
	octet value[size];

	GuidPrefix()
	{
		memset(value, 0, size);
	}

	static GuidPrefix unknow()
	{
		return GuidPrefix();
	}

	bool operator==(const GuidPrefix& prefix) const
	{
		return (memcmp(value, prefix.value, size) == 0);
	}

	bool operator!=(const GuidPrefix& prefix) const
	{
		return (memcmp(value, prefix.value, size) != 0);
	}

	bool operator<(const GuidPrefix& prefix) const
	{
		return std::memcmp(value, prefix.value, size) < 0;
	}

	static int cmp(const GuidPrefix& prefix1, const GuidPrefix& prefix2)
	{
		return std::memcmp(prefix1.value, prefix2.value, size);
	}

};

const GuidPrefix GuidPrefix_Unknown;

inline std::ostream& operator<<(std::ostream& output, const GuidPrefix& guiP)
{
	output << std::hex;
	char old_fill = output.fill('0');
	for (uint8_t i = 0; i < 11; ++i)
	{
		output << std::setw(2) << (int)guiP.value[i] << ".";
	}
	output << std::setw(2) << (int)guiP.value[11];
	output.fill(old_fill);
	return output << std::dec;
}

inline std::istream& operator>>(std::istream& input, GuidPrefix& guiP)
{
	std::istream::sentry s(input);
	if (s)
	{
		char point;
		unsigned char hex;
		std::ios_base::iostate excp_mask = input.exceptions();
		try
		{
			input.exceptions(excp_mask | std::ios_base::failbit 
					| std::ios_base::badbit);
			input >> std::hex >> hex;

			if (hex > 255)
			{
				input.setstate(std::ios_base::failbit);
			}

			guiP.value[0] = static_cast<octet>(hex);

			for (int i = 0; i < 12; ++i)
			{
				input >> point >> hex;
				if (point != '.' || hex > 255)
				{
					input.setstate(std::ios_base::failbit);
				}
				guiP.value[i] = static_cast<octet>(hex);
			}
			input >> std::dec;
		}
		catch (std::ios_base::failure&)
		{
			guiP = GuidPrex::unknown();
		}
		input.exception(excp_mask);
	}
	return input;
}

struct GUID 
{
	GuidPrefix guid_prefix;
	EntityId entity_id;
};

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi

#endif
