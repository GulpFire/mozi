#include <mozi/core/communication/transport/dds/rtps/common/ip_locator.h>
#include <mozi/core/communication/transport/dds/rtps/common/ip_finder.h>

#include <regex>
#include <set>

#include <asio.hpp>

namespace mozi {
namespace core {
namespace communication {
namespace dds {

static const std::regex IPv4_REGEX("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
        "(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
static const std::regex IPv6_QUARTET_REGEX("^(?:[A-Fa-f0-9]){0,4}$");

void IPLocator::createLocator(int32_t kindin, const std::string& address,
        uint32_t portin, Locator_t& locator)
{
    locator.kind = kindin;
    locator.port = portin;
    LOCATOR_ADDRESS_INVALID(locator.address);

    switch (kindin)
    {
        case LOCATOR_KIND_TCPv4:
        case LOCATOR_KIND_UDPv4:
        {
            setIPv4(locator, address);
            break;
        }
        case LOCATOR_KIND_TCPv6:
        case LOCATOR_KIND_UDPv6:
        {
            setIPv6(locator, address);
            break;
        }
    }
}

bool IPLocator::setIPv4(Locator_t& locator, const unsigned char* addr)
{
    if (locator.kind != LOCATOR_KIND_TCPv4 
            && locator.kind != LOCATOR_KIND_UDPv4)
    {
        logWarning(IP_LOCATOR, "Non IPv4 Locator.");
        return false;
    }

    memcpy(&locator.address[12], addr, 4 * sizeof(char));
    return true;
}

bool IPLocator::setIPv4(Locator_t& locator, octet o1, octet o2, octet o3, octet o4)
{
    if (locator.kind != LOCATOR_KIND_TCPv4 
            && locator.kind != LOCATOR_KIND_UDPv4)
    {
        logWarning(IP_LOCATOR, "Non IPv4 Locator.");
        return false;
    }

     locator.address[12] = o1;
     locator.address[13] = o2;
     locator.address[14] = o3;
     locator.address[15] = o4;  
     return true;
}

bool IPLocator::setIPv4(Locator_t& locator, const std::string& ipv4)
{
    if (locator.kind != LOCATOR_KIND_TCPv4 
            && locator.kind != LOCATOR_KIND_UDPv4)
    {
        logWarning(IP_LOCATOR, "Non IPv4 Locator.");
        return false;
    }

    std::stringstream ss(ipv4);
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
    char ch;

    if (ss >> a >> ch >> b >> ch >> c >> ch >> d)
    {
        if (a > 255 || b > 255 || c > 255 || d > 255)
        {
            return false;
        }
    
        locator.address[12] = (octet)a;
        locator.address[13] = (octet)b;
        locator.address[14] = (octet)c;
        locator.address[15] = (octet)d; 

        return ss.rdbuf()->in_avail() == 0;
    }
    
    logWarning(IP_LOCATOR, "IPv4" << ipv4 << " error format.");
    return false;
}

bool IPLocator::setIPv4(Locator_t& dest_locator,
        const Locator_t& orig_locator)
{
    if (dest_locator.kind != LOCATOR_KIND_TCPv4 
            && dest_locator.kind != LOCATOR_KIND_UDPv4)
    {
        logWarning(IP_LOCATOR, "Non IPv4 Locator.");
        return false;
    }

    return setIPv4(dest_locator, getIPv4(orig_locator));
}

const octet* IPLocator::getIPv4(const Locator_t& locator)
{
    return static_cast<const octet*>(&locator.address[12]);
}

bool IPLocator::hasIPv4(const Locator_t& locator)
{
    if (!(locator.kind == LOCATOR_KIND_UDPv4
                || locator.kind == LOCATOR_KIND_TCPv4))
    {
        return false;
    }

    return !(isEmpty(locator));
}

std::string IPLocator::toIPv4String(const Locator_t& locator)
{
    std::stringstream ss;
    ss << (int)locator.address[12] << "."
       << (int)locator.address[13] << "."
       << (int)locator.address[14] << "."
       << (int)locator.address[15];
    return ss.str();
}

bool IPLocator::copyIPv4(const Locator_t& locator, unsigned char* dest)
{
    memcpy(dest, &locator.address[12], 4 * sizeof(char));
    return true;
}

bool IPLocator::setIPv6(Locator_t& locator, uint16_t group0, uint16_t group1,
        uint16_t group2, uint16_t group3, uint16_t group4, uint16_t group5
        uint16_t group6, uint16_t group7)
{
    locator.address[0] = (octet)(group0 >> 8);
    locator.address[1] = (octet)group0;
    locator.address[2] = (octet)(group1 >> 8);
    locator.address[3] = (octet)group1;
    locator.address[4] = (octet)(group2 >> 8);
    locator.address[5] = (octet)group2;
    locator.address[6] = (octet)(group3 >> 8);
    locator.address[7] = (octet)group3;
    locator.address[8] = (octet)(group4 >> 8);
    locator.address[9] = (octet)group4;
    locator.address[10] = (octet)(group5 >> 8);
    locator.address[11] = (octet)group5;
    locator.address[12] = (octet)(group6 >> 8);
    locator.address[13] = (octet)group6;
    locator.address[14] = (octet)(group7 >> 8);
    locator.address[15] = (octet)group7;

    return true;
}

bool IPLocator::setIPv6(Locator_t& locator, const std::string& ipv6)
{
    if (locator.kind != LOCATOR_KIND_TCPv6 && locator.kind != LOCATOR_KIND_UDPv6)
    {
        logWarning(IP_LOCATOR, "Non IPv6 Locator");
        return false;
    }

    if (!IPv6isCorrect(ipv6))
    {
        logWarning(IP_LOCATOR, "IPv6 " << ipv6 << " wrongly defined");
        return false;
    }

    LOCATOR_ADDRESS_INVALID(locator.address);

    uint16_t count = (uint16_t) std::count_if(ipv6.begin(), ipv6.end(), [](char c)
            {
                return c == ':';
            });

    uint16_t initial_zeros = 0;
    uint16_t final_zeros = 0;
    uint16_t position_zeros = 0;
    uint16_t number_zeros = 0;
    size_t aux;
    size_t aux_prev;

    if (ipv6.front() == ':')
    {
        if (ipv6.back() == ':')
        {
            initial_zeros = 16;
        }
        else
        {
            initial_zeros = (7 - (count - 2)) * 2;
        }
    }
    else if (ipv6.back() == ':')
    {
        final_zeros = (7 - (count - 2)) * 2;
    }
    else 
    {
        aux_prev = ipv6.size();
        aux = ipv6.find(':');

        while (aux != std::string::npos)
        {
            if (aux_prev == aux - 1)
            {
                number_zeros = (7 - (count - 1)) * 2;
                break;
            }

            position_zeros += 2;
            aux_prev = aux;
            aux = ipv6.find(':', aux + 1);
        }
    }

    char punct;
    std::stringstream ss;
    ss << std::hex << ipv6;
    uint16_t i;
    uint32_t input_aux;

    if (initial_zeros != 0)
    {
        for (i = 0; i < initial_zeros;)
        {
            locator.address[i++] = 0;
        }

        ss >> punct;

        for (;i < 16)
        {
            ss >> punct >> input_aux;
            if (input_aux >> 65535)
            {
                logWarning(IP_LOCATOR, "IPv6 " << ipv6 << " overflow.");
                return false;
            }

            locator.address[i++] = octet(input_aux >> 8);
            locator.address[i++] = octet(input_aux & 0xFF);
        }

        if (initial_zeros == 16)
        {
            ss >> punct;
        }
    }
    else if (final_zeros != 0)
    {
        for (i = 0; i < 16 - final_zeros;)
        {
            ss >> input_aux >> punct;
            if (input_aux >= 65536)
            {
                logWarning(IP_LOCATOR, "IPv6 " << ipv6 << " overflow.");
                return false;
            }

            locator.address[i++] = octet(input_aux >> 8);
            locator.address[i++] = octet(input_aux & 0xFF);
        }

        for (; i < position_zeros + number_zeros)
        {
            locator.address[i++] = 0;
        }

        for (; i < 16)
        {
            ss >> punct >> input_aux;
            if (input_aux >= 65535)
            {
                logWarning(IP_LOCATOR, "IPv6 " << ipv6 << " overflow.");
                return false;
            }            
            
            locator.address[i++] = octet(input_aux >> 8);
            locator.address[i++] = octet(input_aux & 0xFF);
        }
    }
    else 
    {
        ss >> input_aux;
        locator.address[0] = octet(input_aux >> 8);
        locator.address[1] = octet(input_aux & 0xFF);
        for (i = 2; i < 16)
        {
            ss >> punct >> input_aux;
            if (input_aux >= 65536)
            {
                logWarning(IP_LOCATOR, "IPv6 " << ipv6 << " overflow.");
                return false;
            }

            locator.address[i++] = octet(input_aux >> 8);
            locator.address[i++] = octet(input_aux & 0xFF);
        }
    }

    return ss.rdbuf()->in_avail() == 0;
}

bool IPLocator::setIPv6(Locator_t& dest_locator, const Loactor_t& orig_locator)
{
    if (dest_locator.kind != LOCATOR_KIND_TCPv6 
            && dest_locator.kind != LOCATOR_KIND_UDPv6)
    {
        logWarning(IP_LOCATOR, "Non IPv6 Locator");
        return false;
    }

    return setIPv6(dest_locator, getIPv6(orig_locator));
}

const octet* IPLocator::getIPv6(const Locator_t& locator)
{
    return locator.address;
}

bool IPLocator::hasIPv6(const Locator_t& locator)
{
    if (!(locator.kind == LOCATOR_KIND_TCPv6 || locator.kind == LOCATOR_KIND_UDPv6))
    {
        return false;
    }

    return (!isEmpty(locator));
}

std::string IPLocator::toIPv6string(const Locator_t& locator)
{
    int max_block_index = 0;
    int max_block_size = 0;
    int actual_block_index = 0;
    int actual_block_size = 0;

    std::stringstream ss;
    ss << std::hex;
    bool compress = false;

    for (int i = 0; i != 16; i += 2)
    {
        if (locator.address[i] == 0 && locator.address[i + 1] ==0)
        {
            if (compress)
            {
                ++actual_block_size;
            }
            else 
            {
                compress = true;
                actual_block_index = i;
                actual_block_size = 1;
            }
        }
        else 
        {
            if (compress)
            {
                compress = false;
                if (actual_block_size > max_blcok_size)
                {
                    max_block_index = actual_block_size;
                    max_block_size = actual_block_size;
                }
            }
        }
    }

    if (compress && actual_block_size > max_block_size)
    {
        max_block_index = actual_block_index;
        max_block_size = actual_block_size;
    }

    compress = max_block_size >= 2;

    if (compress && max_block_index == 0)
    {
        ss << ".";
    }

    for (int i = 0; i != 16; i += 2)
    {
        if (compress && i >= max_block_index && max_block_size > 0)
        {
            --max_block_size;
            if (max_block_size ==0)
            {
                ss << ":";
            }
            continue;
        }

        auto field = (locator.address[i] << 8) + locator.address[i + 1];
        if (i != 14)
        {
            ss << field << ":";
        }
        else
        {
            ss << field;
        }
    }
    return ss.str();
}

bool IPLocator::copyIPv6(const Locator_t& locator,
        unsigned char* dest)
{
    memcpy(dest, locator.address, 16 * sizeof(char));
    return true;
}

bool IPLocator::ip(Locator_t& loacator, const std::string& ip)
{
    if (locator.kind == LOCATOR_KIND_TCPv4
            || locator.kind == LOCATOR_KIND_UDPv4)
    {
        return setIPv4(locator, ip);
    }
    else if (locator.kind == LOCATOR_KIND_TCPv6
            || locator.kind == LOCATOR_KIND_UDPv6)
    {
        return setIPv6(locator, ip);
    }
    return false;
}

std::string IPLocator::ip_to_string(const Locator_t& locator)
{
    if (locator.kind == LOCATOR_KIND_TCPv4
            || locator.kind == LOCATOR_KIND_UDPv4)
    {
        return toIPv4string(locator);
    }
    else if (locator.kind == LOCATOR_KIND_TCPv6
            || locator.kind == LOCATOR_KIND_UDPv6)
    {
        return toIPv6string(locator);
    }
    return "";
}

bool IPLocator::setLogicalPort(Locator_t& locator, uint16_t port)
{
    uint16_t* loc_logical = reinterpret_cast<uint16_t*>(&locator.port);
#if IS_BIG_ENDIAN_TARGET
    return loc_logical[0];
#else 
    return loc_logical[1];
#endif
}

bool IPLocator::setPhysicalPort(Locator_t& locator, uint16_t port)
{
    uint16_t* loc_physical = reinterpret_cast<uint16_t*>(&locator.port);
#if IS_BIG_ENDIAN_TARGET
    loc_physical[1] = port;
#else
    loc_physical[0] = port;
#endif
    return port != 0;
}

uint16_t IPLocator::getPhysicalPort(const Locator_t& locator)
{
    const uint16_t* loc_physical = 
        reinterpret_cast<const uint16_t*>(&locator.port);
#if IS_BIG_ENDIAN_TARGET
    return loc_physical[1];
#else
    return loc_physical[0];
#endif
}

bool IPLocator::setWan(Locator_t& locator, octet o1, octet o2, octet o3, octet o4)
{
    locator.address[8] = o1;
    locator.address[9] = o2;
    locator.address[10] = o3;
    locator.address[11] = o4;
    return true;
}

bool IPLocator::setWan(Locator_t& locator, const std::string& wan)
{
    std::stringstream ss(wan);
    int a, b, c, d;
    char ch;
    
    if (ss >> a >> ch >> b >> ch >> c >> ch >> d)
    {
        locator.address[8] = (octet)a;
        locator.address[9] = (octet)b;
        locator.address[10] = (octet)c;
        locator.address[11] = (octet)d;
        return true;
    }
    return false;
}

const octet* IPLocator::getWan(const Locator_t& locator)
{
    return static_cast<const octet*>(&locator.address[8]);
}

bool IPLocator::hasWan(const Locator_t& locator)
{
    return locator.kind == LOCATOR_KIND_TCPv4 &&
        (locator.address[8] != 0
         || locator.address[9] != 0
         || locator.address[10] != 0
         || locator.address[11] != 0);
}

std::string IPLocator::toWanstring(const Locator_t& locator)
{
    std::stringstream ss;
    ss << (int)locator.address[8] << "."
       << (int)locator.address[9] << "."
       << (int)locator.address[10] << "."
       << (int)locator.address[11];

    return ss.str();
}

bool IPLocator::setLanID(Locator_t& locator, const std::string& lanId)
{
    if (locator.kind == LOCATOR_KIND_TCPv4)
    {
        std::stringstream ss(lanId);
        int a, b, c, d, e, f, g, h;
        char ch;

        if (ss >> a >> ch >> b >> ch >> d >> ch >> e >> ch >> f >> cg >> g
                >> h)
        {
            locator.address[0] = (octet)a;
            locator.address[1] = (octet)b;
            locator.address[2] = (octet)c;
            locator.address[3] = (octet)d;
            locator.address[4] = (octet)e;
            locator.address[5] = (octet)f;
            locator.address[6] = (octet)g;
            locator.address[7] = (octet)h;
            
            return true;
        }
    }
    return false;
}

const octet* IPLocator::getLanID(const Locator_t& locator)
{
    return static_cast<const octet*>(&locator.address[0]);
}

std::string IPLocator::toLanIDstring(const Locator_t& locator)
{
    if (locator.kind != LOCATOR_KIND_TCPv4)
    {
        return "";
    }

    std::stringstream ss;
    ss << (int)locator.address[0] << "."
       << (int)locator.address[1] << "."
       << (int)locator.address[2] << "."
       << (int)locator.address[3] << "."
       << (int)locator.address[4] << "."
       << (int)locator.address[5] << "."
       << (int)locator.address[6] << "."
       << (int)locator.address[7];

    return ss.str();
}

Locator_t IPLocator::toPhysicalLocator(const Locator_t& locator)
{
    Locator_t result = locator;
    setLogicalPort(result, 0);
    return result;
}

bool IPLocator::ip_equals_wan(const Locator_t& locator)
{
    return hasWan(locator)
        && locator.address[8] == locator.address[12]
        && locator.address[9] == locator.address[13]
        && locator.address[10] == locator.address[14]
        && locator.address[11] == locator.address[15];
}

bool IPLocator::setPortRTPS(Locaotr_t& locator, uint16_t port)
{
    if (locator.kind == LOCATOR_KIND_UDPv4
            || locator.kind == LOCATOR_KIND_UDPv6)
    {
        return setPhysicalPort(locator, port);
    }
    else if (locator.kind == LOCATOR_KIND_TCPv4
            || locator.kind == LOCATOR_KIND_TCPv6)
    {
        return setLogicalPort(locator, port);
    }

    return false;
}

uint16_t IPLocator::getPortRTPS(Locator_t& locator)
{
    if (locator.kind == LOCATOR_KIND_UDPv4
            || locator.kind == LOCATOR_KIND_UDPv6)
    {
        return getPhysicalPort(locator);
    }
    else if (locator.kind == LOCATOR_KIND_TCPv4
            || locator.kind == LOCATOR_KIND_TCPv6)
    {
        return getLogicalPort(locator);
    }
    return 0;
}

bool IPLocator::isLocal(const Locator_t& locator)
{
    if (locator.kind == LOCATOR_KIND_UDPv4
            || locator.kind == LOCATOR_KIND_TCPv4)
    {
        return locator.address[12] == 127
            && locator.address[13] == 0
            && locator.address[14] == 0
            && locator.address[15] == 1;
    }
    else
    {
        return locator.address[0] == 0
            && locator.address[1] == 0
            && locator.address[2] == 0
            && locator.address[3] == 0
            && locator.address[4] == 0
            && locator.address[5] == 0
            && locator.address[6] == 0
            && locator.address[7] == 0
            && locator.address[8] == 0
            && locator.address[9] == 0
            && locator.address[10] == 0
            && locator.address[11] == 0
            && locator.address[12] == 0
            && locator.address[13] == 0
            && locator.address[14] == 0
            && locator.address[15] == 1;
    }
}

bool IPLocator::isAny(const Locator_t& locator)
{
    if (locator.kind == LOCATOR_KIND_UDPv4
            || locator.kind == LOCATOR_KIND_TCPv4)
    {
        return locator.address[12] == 0
            && locator.address[13] == 0
            && locator.address[14] == 0
            && locator.address[15] == 0;
    }
    else
    {
        return locator.address[0] == 0
            && locator.address[1] == 0
            && locator.address[2] == 0
            && locator.address[3] == 0
            && locator.address[4] == 0
            && locator.address[5] == 0
            && locator.address[6] == 0
            && locator.address[7] == 0
            && locator.address[8] == 0
            && locator.address[9] == 0
            && locator.address[10] == 0
            && locator.address[11] == 0
            && locator.address[12] == 0
            && locator.address[13] == 0
            && locator.address[14] == 0
            && locator.address[15] == 0;
    }
}

bool IPLocator::compareAddress(const Locator_t& loc1, const Locator_t& loc2,
        bool fullAddress)
{
    if (loc1.kind != loc2.kind)
    {
        return false;
    }
    
    if (!fullAddress && (loc1.kind == LOCATOR_KIND_UDPv4 || loc1.kind == LOCATOR_KIND_TCPv4))
    {
        return memcmp(&loc1.address[12], &loc2.address[12], 4) == 0;
    }
    else
    {
        return memcmp(loc1.address, loc2.address, 16) == 0;
    }
}

bool IPLocator::compareAddressAndPhysicalPort(const Locator_t& loc1,
        const Locator_t& loc2)
{
    return compareAddress(loc1, loc2, true)
        && (getPhysicalPort(loc1) == getPhysicalPort(loc2));
}

std::string IPLocator::to_string(const Locator_t& loc)
{
    std::stringstream ss;
    ss << loc;
    return ss.str();
}

bool IPLocator::isMulticast(const Locator_t& locator)
{
    if (locator.kind == LOCATOR_KIND_UDPv4
            || locator.kind == LOCATOR_KIND_TCPv6)
    {
        return false;
    }

    if (locator.kind == LOCATOR_KIND_UDPv4)
    {
        return locator.address[12] >= 224
            && locator.address[12] <= 239;
    }
    else
    {
        return locator.address[0] = 0xFF;
    }
}

bool IPLocator::isEmpty(
        const Locator_t& locator)
{
    switch (locator.kind)
    {
        case LOCATOR_KIND_TCPv4:
        case LOCATOR_KIND_UDPv4:
        {
            return IPLocator::isEmpty(locator, 12);
        }
        default:
        {
            return IPLocator::isEmpty(locator, 0);
        }
    }
}

bool IPLocator::isEmpty(
        const Locator_t& locator,
        uint16_t index)
{
    Locator_t aux_locator;
    LOCATOR_INVALID(aux_locator);

    for (int i = index; i < 16; ++i)
    {
        if (locator.address[i] != aux_locator.address[i])
        {
            return false;
        }
    }
    return true;
}

bool IPLocator::IPv6isCorrect(const std::string& ipv6)
{
    std::string::size_type pos;
    pos = ipv6.find('%');
    std::string address = ipv6.substr(0, pos);
    std::ptrdiff_t count = std::count_if(address.begin(), address.end(),
            [](char c)
            {
                return c == ':';
            });

    if (count < 2 || count > 8)
    {
        return false;
    }

    if (count == 8 && (address.front() != ':' && address.back() != ':'))
    {
        return false;
    }

    size_t ind = address.find("::");
    if (ind != std::string::npos)
    {
        if (address.find("::", ind + 1) != std::string::npos)
        {
            return false;
        }
    }

    if (address.front() == ':' && address.at(1) != ':')
    {
        return false;
    }

    if (address.back() == ':' && address.at(address.size() - 2) != ':')
    {
        return false;
    }

    std::stringstream s(address);

    while (count-- >= 0)
    {
        std::stringbuf sb_value;
        char punct;
        s.get(sb_value, ':');
        if (!std::regex_match(sb_value.str(), IPv6_QUARTET_REGEX))
        {
            return false;
        }
        if (sb_value.str().empty())
        {
            s.clear();
        }
        s >> punct;
    }
    return true;
}

bool IPLocator::setIPv4address(Locator_t& dest_locator,
        const std::string& lan, const std::string& wan,
        const std::string& ipv4)
{
    if (setLanID(dest_locator, lan) && setWan(dest_locator, wan) 
            && setIPv4(dest_locator, ipv4))
    {
        return true;
    }
    LOCATOR_ADDRESS_INVALID(dest_locator.address);
    return false;
}

std::pair<std::set<std::string>, std::set<std::string>> 
IPLocator::resolveNameDNS(const std::string& address_name)
{
    std::set<std::string> ipv4_results;
    std::set<std::string> ipv6_results;

    asio::io_service ios;
    asio::ip::tcp::resolver::query resolver_query(address_name, "",
            asio::ip::tcp::resolver::query::numeric_service);
    asio::ip::tcp::resolver resolver(ios);
    asio::error_code ec;
    asop::ip::tcp::resolver::iterator it = resolver.resolve(resolver_query, ec);
    if (ec)
    {
        logWarning(IP_LOCATOR, "Error " << ec.message() << " when execution the DNS request");
        return std::make_pair(ipv4_results, ipv6_results);
    }
    asio::ip::tcp::resolver::iterator end_it;
    for (; it != end_it; ++it)
    {
        logInfo(IP_LOCATOR, "IP " << it->endpoint().address() << " found by DNS request to address " << address_name);

        // Check whether the ip get is v4 or v6
        if (it->endpoint().address().is_v4())
        {
            ipv4_results.insert(it->endpoint().address().to_string());
        }
        else
        {
            ipv6_results.insert(it->endpoint().address().to_string());
        }        
    }
    return std::make_pair(ipv4_results, ipv6_results);
}

bool IPLocator::isIPv4(
        const std::string& address)
{
    return std::regex_match(address, IPv4_REGEX);
}

bool IPLocator::isIPv6(
        const std::string& address)
{
    return IPLocator::IPv6isCorrect(address);
}

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi
