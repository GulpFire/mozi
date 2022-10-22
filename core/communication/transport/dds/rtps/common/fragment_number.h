#ifndef MOZI_CORE_COMMUNICATION_DDS_RTPS_COMMON_FRAGMENT_NUMBER_H
#define MOZI_CORE_COMMUNICATION_DDS_RTPS_COMMON_FRAGMENT_NUMBER_H

#include <mozi/core/common/data_structure/fixed_size_bitmap.h>
#include <mozi/core/communication/transport/dds/rtps/common/types.h>

#include <algorithm>
#include <cmath>
#include <set>

namespace mozi {
namespace core {
namespace communication {
namespace dds {

using FragmentNumber = uint32_t;
using FragmentNumberSet = BitmapRange<FragmentNumber>;
inline std::ostream& operator<<(std::ostream& output, 
		const FragmentNumberSet& fns)
{
	output << fns.base() << ":";
	fns.for_each([&](FragmentNumber it)
			{
				output << it << "-";
			});
	return output;
}

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi

#endif
