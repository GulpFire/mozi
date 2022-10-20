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

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi
