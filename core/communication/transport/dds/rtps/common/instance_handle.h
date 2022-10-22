#ifndef MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_COMMON_INSTANCE_HANDLE_H
#define MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_COMMON_INSTANCE_HANDLE_H

#include <array>

#include <mozi/core/communication/transport/dds/rtps/common/types.h>
#include <mozi/core/communication/transport/dds/rtps/common/guid.h>

namespace mozi {
namespace core {
namespace communication {
namespace dds {
using KeyHash = std::array<octet, 16>;

struct InstanceHandleValue
{

};

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi

#endif
