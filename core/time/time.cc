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
	auto now = high_resolution_clock::now();
	auto nano_time_point = 
		std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
	auto epoch = nano_time_point.time_since_epoch();
	uint64_t now_nano = 
		std::chrono::duration_cast<std::chrono::nanoseconds>(epoch).count();
	return Time(now_nano);
}

Time Time::MonoTime()
{
	auto now = steady_clock::now();
	auto nano_time_point =
		std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
	auto epoch = nano_time_point.time_since_epoch();
	uint64_t now_nano = 
		std::chrono::duration_cast<std::chrono::nanoseconds>(epoch).count();
	return Time(now_nano);
}

double Time::ToSecond() const
{
	return static_cast<double>(nanoseconds_) / 1000000000UL;
}

bool Time::IsZero() const
{
	return nanoseconds_ = 0;
}

uint64_t Time::ToNanosecond() const
{
	return nanoseconds_;
}

uint64_t Time::ToMicrosecond() const
{
	return static_cast<uint64_t>(nanoseconds_ / 1000.0);
}

std::string Time::ToString() const
{
	auto nano = std::chrono::nanoseconds(nanoseconds_);
	system_clock::time_point tp(nano);
	auto time = system_clock::to_time_t(tp);
	struct tm stm;
	auto ret = localltime_r(&time, &stm);
	if (ret == nullptr)
	{
		return std::to_string(static_cast<double>(nanoseconds_) / 1000000000.0);
	}

	std::stringstream ss;
	ss << std::put_time(ret, "%F %T");
	ss << "." << std::setw(9) << std::setfill('0') << nanoseconds_ % 1000000000UL;
	return ss.str();
}

void Time::SleepUntil(const Time& time)
{
	auto nano = std::chrono::nanoseconds(time.ToNanosecond());
	system_clock::time_point tp(nano);
	std::this_thread::sleep_until(tp);
}

Duration Time::operator-(const Time& rhs) const
{
	return Duration(static_cast<int64_t>(nanoseconds_ - rhs.nanoseconds_));
}

Time Time::operator+(const Duration& rhs) const
{
	return Time(nanoseconds_ + rhs.ToNanosecond());
}

Time Time::operator-(const Duration& rhs) const
{
	return Time(nanoseconds_ - rhs.ToNanosecond());
}

Time& Time::operator+=(const Duration& rhs)
{
	*this = *this + rhs;
	return *this;
}

Time& Time::operator-=(const Duration& rhs)
{
	*this = *this - rhs;
	return *this;
}

bool Time::operator==(const Time& rhs) const
{
	return nanoseconds_ == rhs.nanoseconds_;
}

bool Time::operator!=(const Time& rhs) const
{
	return nanoseconds_ != rhs.nanoseconds_;
}

bool Time::operator>(const Time& rhs) const
{
	return nanoseconds_ > rhs.nanoseconds_;
}

bool Time::operator<(const Time& rhs) const
{
	return nanoseconds_ < rhs.nanoseconds_;
}

bool Time::operator>=(const Time& rhs) const
{
	return nanoseconds_ >= rhs.nanoseconds_;
}

bool Time::operator<=(const Time& rhs) const
{
	return nanoseconds_ <= rhs.nanoseconds_;
}

std::ostream& operator<<(std::ostream& os, const Time& rhs)
{
	os << rhs.ToString();
	return os;
}

} // namespace core
} // namespace mozi
