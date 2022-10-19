#ifndef MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_LOCATOR_H
#define MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_LOCATOR_H

#include "types.h"

#include <regex>
#include <set>
#include <string>
#include <vector>

namespace mozi {
namespace core {
namespace communication {
namespace dds {

#define LOCATOR_INVALID(loc)            \
{                                       \
    loc.kind = LOCATOR_KIND_INVALID;    \
    loc.port = LOCATOR_PORT_INVALID;    \
    LOCATOR_ADDRESS_INVALID(loc.address);   \
}                                       \

#define LOCATOR_KIND_INVALID -1

#define LOCATOR_ADDRESS_INVALID(a)      \
{                                       \
    std::memset(a, 0x00, 16 * sizeof(octet));   \
}                                       \

#define LOCATOR_PORT_INVALID    0
#define LOCATOR_KIND_RESERVED   0
#define LOCATOR_KIND_UDPv4      1
#define LOCATOR_KIND_UDPv6      2
#define LOCATOR_KIND_TCPv4      4
#define LOCATOR_KIND_TCPv6      8
#define LOCATOR_KIND_SHM        16

class Locator;

class IPLocator 
{
public:
    static void createLocator(int32_t kindin, const std::string& address,
            uint32_t portin, Locator& locator);
    static bool setIPv4(Locator& locator, const unsigned char* addr);
    static bool setIPv4(Locator& locator, 
            octet o1, octet o2, octet o3, octet o4);
    static bool setIPv4(Locator& locator, const std::string& ipv4);
    static bool setIPv4(Locator& destlocator, const Locator_t& origlocator);
    static bool setIPv4address(Locator& destlocator, const std::string& lan,
            const std::string& wan, const std::string& ipv4);
    static const octet* getIPv4(const Locator& locator);
    static bool hasIPv4(const Locator& locator);
    static std::string toIPv4string(const Locator& locator);
    static bool copyIPv4(const Locator& locator, unsigned char* dest);
}

class Locator
{
    public:
        int32_t kind;
        uint32_t port;
        octet address[16];  // IP address;

        Locator()
            : kind(LOCATOR_KIND_UDPv4)
        {
            port = 0;
            LOCATOR_ADDRESS_INVALID(address);
        }

        // move construct
        Locator(Locator&& loc)
            : kind(loc.kind)
        {
            port = loc.port;
            std::memcpy(address, loc.address, 16 * sizeof(octet));
        }

        // copy construct
        Locator(const Locator& loc)
            : kind(loc.kind)
        {
            port = loc.port;
            std::memcpy(address, loc.address, 16 * sizeof(octet));
        }

        Locator(uint32_t portin)
            : kind(LOCATOR_KIND_UDPv4)
        {
            port = portin;
            LOCATOR_ADDRESS_INVALID(address);
        }

        Locator(int32_t kindin, uint32_t portin)
            : kind(kindin)
            , port(portin)
        {
            LOCATOR_ADDRESS_INVALID(address);
        }
};

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi

#endif
