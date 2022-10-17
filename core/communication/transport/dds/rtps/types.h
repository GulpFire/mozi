#ifndef MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_TYPES_H
#define MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_TYPES_H

#include <cstdint>
#include <stddef.h>
#include <stdint.h>
#include <iostream>

namespace mozi {
namespace core {
namespace communication {
namespace dds {

enum Endianness_t
{
    BIGEND = 0x1,
    LITTLEEND = 0x0
};

typedef enum ReliabilityKind_t
{
    RELIABLE,
    BEST_EFFORT
} ReliabilityKind_t;

typedef enum DurabilityKind_t
{
    VOLATILE,
    TRANSIENT_LOCAL,
    TRANSIENT,
    PERSISTENT
} DurabilityKind_t;

typedef enum EndpointKind_t
{
    READER,
    WRITER
}EndpointKind_t;

typedef enum TopicKind_t
{
    NO_KEY,
    WITH_KEY
}TopicKind_t;

#if BIG_ENDIAN_TARGET
constexpr Endianness_t DEFAULT_ENDIAN = BIGEND;
#else 
constexpr Endianness_t DEFAULT_ENDIAN = LITTLEEND;
#endif

using octet = unsigned char;
using SubmessageFlag = unsigned char;
using BuiltinEndpointSet_t = uint32_t;
using Count_t = uint32_t;

#define BIT0 0x01u
#define BIT1 0x02u
#define BIT2 0x04u
#define BIT3 0x08u
#define BIT4 0x10u
#define BIT5 0x20u
#define BIT6 0x40u
#define BIT7 0x80u

#define BIT(i) (1U << static_cast<unsigned>(i))

struct ProtocolVersion_t
{
    octect major_;
    octect minor_;
    ProtocolVersion_t(octect maj, octect min)
        : major_(maj)
        , minor_(min)
    {}

    bool operator == (const ProtocolVersion_t& v) const
    {
        return major_ == v.major_ && minor_ == v.minor_;
    }

    bool operator != (const ProtocolVersion_t& v) const
    {
        return major_ != v.major_ || minor_ != v.minor_;
    }

};

inline std::ostream& operator <<(std::ostream& output, 
        const ProtocolVersion_t& pv)
{
    return output << static_cast<int>(pv.major_) << "." << static_cast<int>(pv.minor_);
}

const ProtocolVersion_t c_ProtocolVersion;

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi

#endif
