#ifndef MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_IP_LOCATOR_H
#define MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_IP_LOCATOR_H

#include <mozi/core/communication/transport/dds/rtps/common/types.h>
#include <mozi/core/log/logger.h>

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

        static bool setIPv4(Locator_t& locator, const std::string& ipv4);

        static bool setIPv4(Locator_t& dest_locator, 
                const Locator_t& orig_locator);

        static const octet* getIPv4(const Locator_t& locator);

        static bool hasIPv4(const Locator_t& locator);

        static std::string toIPv4String(const Locator_t& locator);

        static bool copyIPv4(const Locator_t& locator, unsigned char* dest);

        static bool setIPv6(Locator_t& locator, uint16_t group0, 
                uint16_t group1, uint16_t group2, uint16_t group3,
                uint16_t group4, uint16_t group5, uint16_t group6,
                uint16_t group7);

        static bool setIPv6(Locator_t& locator, const std::string& ipv6);

        static bool setIPv6(Locator_t& dest_locator, 
                const Locator_t& orig_locator);

        static const octet* getIPv6(const Locator_t& locator);

        static bool hasIPv6(const Locator_t& locator);

        static std::string toIPv6string(const Locator_t& locator);

        static bool copyIPv6(const Loactor_t& locator, unsigned char* dest);

        static bool ip(Locator_t& locator, const std::string& ip);

        static std::string ip_to_string(const Locator_t& locator);

        static bool setLogicalPort(Locator_t& locator, uint16_t port);

        static uint16_t getLogicalPort(const Locator_t& locator);

        static bool setPhysicalPort(Locator_t& locator, uint16_t port);

        static uint16_t getPhysicalPort(const Locator_t& locator);

        static bool setWan(Locator_t& locator, octet o1, octet o2, 
                octet o3, octet o4);

        static bool setWan(Locator_t& locator, const std::string& wan);



};

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi

#endif
