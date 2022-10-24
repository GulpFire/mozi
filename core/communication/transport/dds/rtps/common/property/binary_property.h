#ifndef MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_COMMON_PROPERTY_BINARY_PROPERTY_H
#define MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_COMMON_PROPERTY_BINARY_PROPERTY_H

#include <mozi/core/communication/transport/dds/rtps/common/types.h>

#include <string>
#include <vector>
#include <iostream>
#include <cstdint>

namespace mozi {
namespace core {
namespace communication {
namespace dds {

class BinaryProperty
{
    public:
        BinaryProperty()
            : propogate_(false)
        {}

        BinaryProperty(const BinaryProperty& property)
            : name_(property.name_)
            , value_(property.value_)
            , propagate_(property.propagate_)
        {
            
        }

        BinaryProperty(BinaryProperty&& property)
            : name_(std::move(property.name_))
            , value_(std::move(property.value_))
            , propagate_(property.propagate_)
        {
        
        }

        BinaryProperty(const std::string& name, 
                const std::vector<octet>& value)
            : name_(std::move(name))
            , value_(std::move(value))
        {
        
        }

        BinaryProperty& operator=(const BinaryProperty& property)
        {
            name_ = property.name_;
            value_ = property.value_;
            propagate_ = property.propagate_;
            return *this;
        }

        BinaryProperty& operator=(const BinaryProperty&& property)
        {
            name_ = std::move(property.name_);
            value_ = std::move(property.value_);
            propagate_ = property.propagate_;
            return *this;
        }

        bool operator==(const BinaryProperty& bp) const
        {
            return (this->name_ == bp.name_)
                && (this->value_ == bp.value_);
        }

        void name(const std::string& name)
        {
            name_ = name;
        }

        void name(std::string&& name)
        {
            name_ = std::move(name);
        }

        const std::string& name() const
        {
            return name_;
        }

        std::string& name()
        {
            return name_;
        }

        void value(const std::vector<octet>& value)
        {
            value_ = value;
        }

        void value(std::vector<octet>&& value)
        {
            value_ = std::move(value);
        }

        const std::vector<octet>& value() const
        {
            return value_;
        }

        std::vector<octet>& value()
        {
            return value_;
        }

        void propagate(bool propagate)
        {
            propagate_ = propagate;
        }

        bool propagate() const
        {
            return propagate_;
        }

        bool& propagate()
        {
            return propagate_;
        }

    private:
        std::string name_;
        std::vector<octet> value_;
        bool propagate_;
};

using BinaryPropertySequence = std::vector<BinaryProperty>;

class BinaryPropertyHelper
{
    public:
        static size_t serialized_size(const BinaryProperty& binary_property,
                size_t current_alignment = 0)
        {
            if (binary_property.propagate())
            {
                size_t initial_alignment = current_alignment;
                current_alignment += 4 + alignment(current_alignment, 4)
                    + binary_property.name().size() + 1;
                current_alignment += 4 + alignment(current_alignment, 4)
                    + binary_property.value().size();

                return current_alignment - initial_alignment;
            }
            else
            {
                return 0;
            }
        }

        static size_t serialized_size(
                const BinaryPropertySequence& binary_properties,
                size_t current_alignment = 0)
        {
            size_t initial_alignment = current_alignment;
            current_alignment += 4 + alignment(current_alignment, 4);
            for (auto binary_property = binary_properties.begin();
                    binary_property != binary_properties.end();
                    ++binary_property)
            {
                current_alignment += 
                    serialized_size(*binary_property, current_alignment);
            }
            return current_alignment - initial_alignment;
        }
    private:
        inline static size_t alignment(size_t current_alignment, 
                size_t data_size)
        {
            return (data_size - (current_alignment % data_size))
                & (data_size - 1);
        }
};

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi

#endif
