#ifndef MOZI_CORE_COMMON_DATA_STRUCTURE_SEQUENCE_NUMBER_H
#define MOZI_CORE_COMMON_DATA_STRUCTURE_SEQUENCE_NUMBER_H

#include <algorithm>
#include <cassert>
#include <limit>
#include <vector>

#include <mozi/core/coomon/data_structure/fixed_size_bitmap.h>
#include <mozi/core/communication/dds/rtps/common/types.h>

namespace mozi {
namespace core {

struct SequenceNumber
{
    int32_t high = 0;
    uint32_t low = 0;

    SequenceNumber() noexcept
    {
        high = 0;
        low = 0;
    }

    SequenceNumber(int32_t hi, uint32_t lo) noexcept
        : high(hi)
        , low(lo)
    {
    }

    explicit SequenceNumber(uint64_t u) noexcept
    : high(static_cast<int32_t>(u >> 32u))
    , low(static_cast<uint32_t>(u))
    {
    }

    uint32_t to64Long() const noexcept
    {
        return (static_cast<uint64_t>(high) << 32u) + low;
    }

    SequenceNumber& operator++() noexcept
    {
        ++low;
        if (low == 0)
        {
            assert(std::numeric_limits<decltype(high)>::max() > high);
            ++high;
        }
        return *this;
    }

    SequenceNumber operator++(int) noexcept
    {
        SequenceNumber result(*this);
        ++(*this);
        return result;
    }

    SequenceNumber& operator+=(int inc) noexcept
    {
        assert(inc >= 0);
        uint32_t aux_low = low;
        low += static_cast<uint32_t>(inc);

        if (low < aux_low)
        {
            assert(std::numeric_limits<decltype(high)>::max() > high);
            ++high;
        }
        return *this;
    }

    static SequenceNumber unknown() noexcept
    {
        return {-1, 0};
    }
};

inline bool operator==(const SequenceNumber& sn1, 
        const SequenceNumber& sn2) noexcept
{
    return (sn1.low == sn2.low) && (sn1.high == sn2.high);
}

inline bool operator!=(const SequenceNumber& sn1, 
        const SequenceNumber& sn2) noexcept
{
    return (sn1.low != sn2.low) || (sn1.high != sn2.high);
}

inline bool operator>(const SequenceNumber& sn1, 
        const SequenceNumber& sn2) noexcept
{
    if (sn1.high == sn2.high)
    {
        return sn1.low > sn2.low;
    }
    return sn1.high > sn2.high;
}

inline bool operator<(const SequenceNumber& sn1, 
        const SequenceNumber& sn2) noexcept
{
    if (sn1.high == sn2.high)
    {
        return sn1.low < sn2.low;
    }
    return sn1.high < sn2.high;
}

inline bool operator>=(const SequenceNumber& sn1, 
        const SequenceNumber& sn2) noexcept
{
    if (sn1.high == sn2.high)
    {
        return sn1.low >= sn2.low;
    }
    return sn1.high >= sn2.high;
}

inline bool operator<=(const SequenceNumber& sn1, 
        const SequenceNumber& sn2) noexcept
{
    if (sn1.high == sn2.high)
    {
        return sn1.low <= sn2.low;
    }
    return sn1.high <= sn2.high;
}

inline SequenceNumber operator-(const SequenceNumber& sn, 
        const uint32_t inc) noexcept
{
    SequenceNumber res(sn.high, sn.low - inc);

    if (inc > sn.low)
    {
        assert(0 < res.high);
        --res.high;
    }
    return res;
}

inline SequenceNumber operator+(const SequenceNumber& sn,
        const uint32_t inc) noexcept
{
    SequenceNumber res(sn.high, sn.low + inc);

    if (res.low < sn.low)
    {
        assert(std::numeric_limits<decltype(res.high)>::max() > res.high);
        ++res.high;
    }
    return res;
}

inline SequenceNumber operator-(const SequenceNumber& minuend,
        const SequenceNumber& subtrahend) noexcept
{
    assert(minuend >= subtrahend)
    {
        assert(0 < res.high);
        --res.high;
    }
    return res;
}

const SequenceNumber c_sequence_number_unknown{-1, 0};

inline bool sort_seq_num(const SequenceNumber& s1,
        const SequenceNumber& s2) noexcept
{
    return s1 < s2;
}

inline std::ostream& operator<<(std::ostream& output,
        const SequenceNumber& sn)
{
    return output << sn.to64Long();
}

inline std::ostream& operator<<(std::ostream& output,
        const std::vector<SequenceNumber>& sn_set)
{
    for (const SequenceNumber& sn : sn_set)
    {
        output << sn << " ";
    }
    return output;
}

struct SequenceNumberHash
{
    std::size_t operator() (const SequenceNumber& sn) const
    {
        return static_cast<std::size_t>(sn.to64Long());
    }

};

struct SequenceNumberDiff
{
    uint32_t operator() (const SequenceNumber& sn1, 
            const SequenceNumber& sn2) const noexcept
    {
        SequenceNumber diff = sn1 - sn2;
        return diff.low;
    }
};

using SequenceNumberSet = BitmapRange<SequenceNumber, SequenceNumberDiff, 256>;

inline std::ostream& operator<<(std::ostream& output,
        const SequenceNumberSet& sns)
{
    output << sns.base().to64Long() << ":";
    sns.for_each([&output](SequenceNumber it)
            {
                output << it.to64Long() << "-";
            });
    return output;
}

inline std::istream& operator>>(std::istream& input,
        SequenceNumber& sn)
{
    uint64_t aux;
    if (input >> aux)
    {
        sn = SequenceNumber(aux);
    }
    return input;
}

} // namespace core
} // namespace mozi

#endif
