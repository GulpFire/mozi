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

} // namespace
} // namespace communication
} // namespace core
} // namespace mozi

#endif
