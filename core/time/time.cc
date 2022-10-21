#include <mozi/core/time.time.h>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <limits>
#include <sstream>
#include <thread>

namespace mozi {
namespace core {

using std::chrono::high_resolution_clock;
using std::chrono::steady_clock;
using std::chrono::system_clock;

const Time Time::MAX = Time(std::numeric_limits<uint64_t>::max());
const Time Time::MIN = Time(0);

Time::Time(uint64_t nanoseconds)
    : nanoseconds_(nanoseconds)
{

}

Time::Time(int seconds)
    : nanoseconds_(static_cast<uint64_t>(seconds * 1000000000UL))
{

}

Time::Time(uint32_t seconds, uint32_t nanoseconds)
    : nanoseconds_(static_cast<uint64_t>(seconds) * 1000000000UL + nanoseconds)
{

}

Time::Time(const Time& other)
    : nanoseconds_(other.nanoseconds_)
{

}

Time& Time::operator=(const Time& other)
{
    this->nanoseconds_ = other.nanoseconds_;
    return *this;
}

Time Time::Now()
{
    
}

} // namespace core
} // namespace mozi
