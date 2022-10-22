#ifndef MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_COMMON_ENTITY_ID_H
#define MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_COMMON_ENTITY_ID_H

#include <mozi/core/communication/transport/dds/rtps/common/types.h>

#include <cstdint>
#include <cstring>
#include <sstream>

namespace mozi {
namespace core {
namespace communication {
namespace dds {

#define ENTITY_ID_UNKNOWN						0x00000000
#define ENTITY_ID_RTPS_PARTICIPANT	0x000001c1
#define ENTITY_ID_SEDP_BUILTIN_TOPIC_WRITER	0x000002c2
#define ENTITY_ID_SEDP_BUILTIN_TOPIC_READER 0x000002c7
#define ENTITY_ID_SEDP_BUILTIN_PUBLICATIONS_WRITER	0x000003c2
#define ENTITY_ID_SEDP_BUILTIN_PUBLICATIONS_READER 	0x000003c7
#define ENTITY_ID_SEDP_BUILTIN_SUNCRIPTIONS_WRITER	0x000004c2
#define ENTITY_ID_SPDP_BUILTIN_RTPS_PARTICIPANT_WRITER	0x000100c2
#define ENTITY_ID_SPDP_BUILTIN_RTPS_PARTICIPANT_READER	0x000100c7
#define ENTITY_ID_P2P_BUILTIN_RTPS_PARTICIPANT_MESSAGE_WRITER	0x000200c2
#define ENTITY_ID_P2P_BUILTIN_RTPS_PARTICIPANT_MESSAGE_READER	0x000200c7
#define ENTITY_ID_P2P_BUILTIN_PARTICIPANT_STATELESS_WRITER	0x000201c3
#define ENTITY_ID_P2P_BUILTIN_PARTICIPANT_STATELESS_READER 	0x000201c4

#define ENTITY_ID_TL_SVC_REQ_WRITER	0x000300c3
#define ENTITY_ID_TL_SVC_REQ_READER 0x000300c4
#define ENTITY_ID_TL_SVC_REPLY_WRITER	0x000301c3
#define ENTITY_ID_TL_SVC_REPLY_READER	0x000301c4

#define ENTITY_ID_DS_SERVER_VIRTUAL_WRITER	0x00030073
#define ENTITY_ID_DS_SERVER_VIRTUAL_READER	0x00030074

struct EntityId
{
	static constexpr unsigned int size = 4;
	octet value[size];

	EntityId()
	{
		*this = ENTITY_ID_UNKNOWN;
	}

	EntityId(uint32_t id)
	{
		memcpy(value, &id, size);
#if !IS_BIG_ENDIAN_TARGET
		reverse();
#endif
	}

	EntityId(const EntityId& id)
	{
		memcpy(value, id.value, size);
	}

	EntityId(Entity&& id)
	{
		memmove(value, id.value, size);
	}

	EntityId& operator=(const EntityId& id)
	{
		memcpy(value, id.value, size);
		return *this;
	}

	EntityId& operator=(EntityId& id)
	{
		memmove(value, id.value, size);
		return *this;
	}

	EntityId& operator=(uint32_t id)
	{
		memcpy(value, &id, size);
#if !IS_BIG_ENDIAN_TARGET
		reverse();
#endif
		return *this;
	}

#if !IS_BIG_ENDIAN_TARGET
	void reverse()
	{
		octet oaux;
		oaux = value[3];
		value[3] = value[0];
		value[0] = oaux;
		oaux = value[2];
		value[2] = value[1];
		value[1] = oaux;
	}
#endif

	uint32_t toUint32() const
	{
		uint32_t res = *reinterpret_cast<const uint32_t*>(value);
#if !IS_BIG_ENDIAN_TARGET
		res = (res >> 24) 
			| ((res >> 8) & 0x0000ff00) 
			| ((res << 8) & 0x00ff0000)
			| (res << 24);
#endif
		return res;
	}

	static EntityId unknown()
	{
		return EntityId();
	}

	bool isReader() const
	{
		return 0x4u & toUint32();
	}

	bool isWriter() const
	{
		return 0x2u & toUint32() && !isReader();
	}

	bool operator<(const EntityId& other) const
	{
		return std::memcmp(value, other.value, size) < 0;
	}

	static int cmp(const EntityId& entity1, const EntityId& entity2)
	{
		return std::memcmp(entity1.value, entity2.value, size);
	}
};

inline bool operator==(EntityId& id1, const uint32_t id2)
{
#if !IS_BIG_ENDIAN_TARGET
	id1.reverse();
#endif
	const bool result = 0 == memcmp(id1.value, &id2, sizeof(id2));
#if !IS_BIG_ENDIAN_TARGET
	id1.reverse();
#endif
	return result;
}

inline bool operator==(const EntityId& id1, const EntityId& id2)
{
	return EntityId::cmp(id1, id2) == 0;
}

inline bool operator!=(const EntityId& id1, const EntityId& id2)
{
	return !(operator==(id1, id2));
}

inline std::ostream& operator<<(std::ostream& output, const EntityId& id)
{
	output << std::hex;
	output << (int)id.value[0] << "." << (int)id.value[1] << "."
		<< (int)id.value[2] << "." << (int)id.value[3];
	return output << std::dec;
}

inline std::istream& operator>>(std::istream& input, EntityId& id)
{
	std::istream::sentry s(input);
	if (s)
	{
		char point;
		unsigned short hex;
		std::ios_base::iostate excp_mask = input.exception();

		try
		{
			input.exceptions(excp_mask | std::ios_base::failbit 
					| std::ios_base::badbit);
			if (hex > 255)
			{
				input.setstate(std::ios_base::failbit);
			}

			id.value[0] = static_cast<octet>(hex);

			for (int i = 1; i < 4; ++i)
			{
				input >> point >> hex;
				if (point != '.' || hex > 255)
				{
					input.setstate(std::ios_base::failbit)
				}
				id.value[i] = static_cast<octet>(hex);
			}
			input >> std::dec;
		}
		catch (std::ios_base::failure&)
		{}
		input.exceptions(excp_mask);
	}
	return input;
}

const EntityId c_Entity_Id_Unknown = ENTITY_ID_UNKNOWN;
const EntityId c_Entity_Id_SPDP_READER = 
	ENTITY_SPDP_BUILTIN_RTPS_PARTICIPANT_READER;
const EntityId c_Entity_Id_SPDP_WRITER =
	ENTITY_SPDP_BUILTIN_RTPS_PARTICIPANT_WRITER;

const EntityId c_Entity_Id_SEDP_PUB_WRITER =
	ENTITY_ID_SEDP_PUBLICATIONS_WRITER;
const EntityId c_Entity_Id_SEDP_PUB_READER =
	ENTITY_ID_SEDP_PUBLICATIONS_READER;
const EntityId c_Entity_Id_SEDP_SUB_WRITER =
	ENTITY_ID_SEDP_SUBSCRIPTIONS_WRITER;
const EntityId c_Entity_Id_SEDP_SUB_READER = 
	ENTITY_ID_SEDP_SUBSCRIPTIONS_READER;
const EntityId c_Entity_Id_RTPS_Participant = 
	ENTITY_ID_RTPS_PARTICIPANT;

const EntityId c_Entity_Id_Writer_Liveness = 
	ENTITY_ID_P2P_BUILTIN_RTPS_PARTICIPANT_MESSAGE_WRITER;
const EntityId c_Entity_Id_Reader_Liveness =
	ENTITY_ID_P2P_BUILTIN_RTPS_PRATICIPANT_MESSAGE_READER;

const EntityId c_Entity_Id_TypeLookup_Request_Writer =
	ENTITY_ID_TL_SVC_REQ_WRITER;
const EntityId c_Entity_Id_TypeLookup_Request_Reader =
	ENTITY_ID_TL_SVC_REQ_READER;
const EntityId c_Entity_Id_TypeLookup_Reply_Writer = 
	ENTITY_ID_TL_SVC_REPLY_WRITER;
const EntityId c_Entity_Id_TypeLookup_Reply_Reader =
	ENTITY_ID_TL_SVC_REPLY_READER;

const EntityId ds_server_virtual_writer = ENTITY_ID_DS_SERVER_VIRTUAL_WRITER;
const EntityId ds_server_virtual_reader = ENTITY_ID_DS_SERVER_VIRTUAL_READER;
} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi

namespace std {
template <>
struct hash<mozi::core::communication::dds::EntityId>
{
	std::size_t operator()(
			const mozi::core::communication::dds::EntityId& k) const
	{
		return (static_cast<size_t>(k.value[0]) << 16)
			| (static_cast<size_t>(k.value[1]) << 8)
			| static_cast<size_t>(k.value[2]);
	}
};
}

#endif
