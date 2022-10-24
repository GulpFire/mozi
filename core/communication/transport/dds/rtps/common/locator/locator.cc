#include "locator.h"
#include "ip_finder.h"

#include "asio.hpp"

#include <regex>
#include <set>

namespace mozi {
namespace core {
namespace communication {
namespace dds {

static const std::regex IPv4_REGEX("^?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
        "(?:25[0-5]|2[0-4][0-9][01]?[0-9][0-9]?)$");

void IPLocator::createLocator(int32_t kindin, const std::string& address,
        uint32_t portin, Locator& locator)
{

}

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi
