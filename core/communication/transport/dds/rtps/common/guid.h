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
struct InstanceHandle;

struct GUID 
{
	GuidPrefix guid_prefix;
	EntityId entity_id;

	GUID() noexcept
	{
	}

	GUID(const GuidPrefix& prefix, uint32_t id)
		: guid_prefix(prefix)
		, entity_id(id)
	{}

	GUID(const GuidPrefix& prefix, const EntityId& eid) noexcept
		: guid_prefix(prefix)
		, entity_id(eid)
		{
		}

	bool isOnSameHostAs(const GUID& other) const
	{
		return memcmp(guid_prefix.value, other.guid_prefix.value, 4) == 0;
	}

	bool isOnSameProcessAs(const GUID& other) const
	{
		return memcmp(guid_prefix.value, other.guid_prefix.value, 8) == 0;
	}

	bool isBuiltin() const
	{
		return entity_id.value[3] >= 0xC0;
	}

	static GUID unknown() noexcept 
	{
		return GUID();
	}

	explicit operator const InstanceHandle&() const
	{
		return *reinterpret_cast<const InstanceHandle*>(this);
	}
};

inline bool operator==(const GUID& g1, const GUID& g2)
{
	if (g1.guid_prefix == g2.guid_prefix && g1.entity_id == g2.entity_id)
	{
		return true;
	}
	else
	{
		return false;
	}
}

inline bool operator!=(const GUID& g1, const GUID& g2)
{
	if (g1.guid_prefix != g2.guid_prefix || g1.entity_id != g2.entity_id)
	{
		return true;
	}
	else
	{
		return false;
	}
}

inline bool operator<(const GUID& g1, const GUID& g2)
{
	auto prefix_cmp = GuidPrefix::cmp(g1.guid_prefix, g2.guid_prefix);
	if (prefix_cmp < 0)
	{
		return true;
	}
	else if (prefix_cmp > 0)
	{
		return false;
	}
	else
	{
		return g1.entity_id < g2.entity_id;
	}
}

const GUID c_Guid_Unknown;

inline std::ostream& operator<<(std::ostream& output, const GUID& guid)
{
	if (guid != c_Guid_Unknown)
	{
		output << guid.guid_prefix << "|" << guid.entity_id;
	}
	else
	{
		output << "|GUID UNKNOWN|"
	}
	return output;
}

inline std::istream& operator>>(std::istream& input, GUID& guid)
{
	std::istream::sentry s(input);
	if (s)
	{
		std::ios_base::iostate excp_mask = input.exceptions();
		try
		{
			input.exceptions(excepp_mask | std::ios_base::failbit
					| std::ios_base::badbit);
			char sep;
			input >> guid.guid_prefix >> sep >> guid.entity_id;

			if (sep != '|')
			{
				input.setstate(std::ios_base::failbit);
			}
		}
		catch (std::ios_base::failure&)
		{
			guid = c_Guid_Unknown;
		}

		input.exceptions(excp_mask);
	}
	return input;
}

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi

#endif
