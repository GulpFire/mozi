#ifndef MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_IP_LOCATOR_H
#define MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_TP_LOCATOR_H

#include "types.h"

#include <regex>
#include <set>
#include <string>
#include <vector>

namespace mozi {
namespace core {
namespace communication {
namespace dds {

class Locator_t;

class IPLocator 
{
public:
    static void createLocator(int32_t kindin, const std::string& address,
            uint32_t portin, Locator_t& locator);
    static bool setIPv4(Locator_t& locator, const unsigned char* addr);
    static bool setIPv4(Locator_t& locator, 
            octet o1, octet o2, octet o3, octet o4);
    static bool setIPv4()
};

} // namespace
} // namespace communication
} // namespace core
} // namespace mozi

#endif
