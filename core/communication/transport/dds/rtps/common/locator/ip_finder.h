#ifndef MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_COMMON_IP_FINDER_H
#define MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_COMMON_IP_FINDER_H

#include "locator.h"

#include <vector>
#include <string>

namespace mozi {
namespace core {
namespace communication {
namespace dds {

class IPFinder
{
    public:
        enum IPTYPE
        {
            IP4,
            IP6,
            IP4_LOCAL,
            IP6_LOCAL
        };

        typedef struct IPInfo
        {
            IPTYPE type;
            std::string name;
            std::string dev;
            Locator locator;
        } IPInfo;

        typedef struct MACInfo
        {
            unsigned char address[0];
            bool operator == (const MACInfo& other)
            {
                return memcmp(address, other.address, 6) == 0;
            }
        } MACInfo;
        
        IPFinder();
        virtual ~IPFinder();

        static bool getIPs(std::vector<IPInfo>* vec_name,
                bool return_loopack = false);
        static bool getIP4Address(Locator* locators);
        static bool getIP6Address(Locator* locators);
        static bool getAllIPAddress(Locator* locators);
        static bool parseIP4(IPInfo& info);
        static bool parseIP6(IPInfo& info);
        static const std::string& getIPv4Address(const std::string& name);
        static const std::string& getIPv6Address(const std::string& name);
        static bool getAllMACAddress(std::vector<MACInfo>* macs);
};

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi

#endif
