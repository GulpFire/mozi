#ifndef MOZI_CORE_TIME_DURATION_H
#define MOZI_CORE_TIME_DURATION_H

#include <cstdint>
#include <iostream>

namespace mozi {
namespace core {

class Duration 
{
    public:
        Duration() = default;
        explicit Duration(int64_t nanoseconds)
        explicit Duration(int nanoseconds);
        explicit Duration(double seconds);
        Duration(uint32_t seconds, uint32_t nanoseconds);
        Duration(const Duration& other);
        Duration& operator=(const Duration& other);
        ~Duration() = default;

        double ToSecond() const;
        int64_t ToNanosecond() const;
        bool IsZero() const;
        void Sleep() const;

        Duration operator+(const Duration& rhs) const;
        Duration operator-(const Duration& rhs) const;
        Duration operator-() const;
        Duration operator*(double scale) const;
        Duration& operator+=(const Duration& rhs);
        Duration& operator-=(const Duration& rhs);
        Duration& operator*=(double scale);
        bool operator==(const Duration& rhs) const;
        bool operator!=(const Duration& rhs) const;
        bool operator>(const Duration& rhs) const;
        bool operator<(const Duration& rhs) const;
        bool operator>=(const Duration& rhs) const;
        bool operator<=(const Duration& rhs) const;

    private:
        int64_t nanoseconds_ = 0;
};

std::ostream& operator<<(std::ostream& os, const Duration& rhs);

} // namespace core
} // namespace mozi

#endif
