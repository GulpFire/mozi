#ifndef MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_COMMON_PROPERTY_PROPERTY_H
#define MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_COMMON_PROPERTY_PROPERTY_H

#include <string>
#include <vector>

namespace mozi {
namespace core {
namespace communication {
namespace dds {

class Property
{
    public:
        Property()
        {
        
        }

        Property(const Property& property)
            : name_(property.name_)
            , value_(property.value_)
            , propagte_(property.propagte)
        {

        }

        Property(Property&& property)
            : name_(std::move(property.name_))
            , value_(std::move(property.value_))
            , propagate_(property.propagate_)
        {
        
        }

        Property(const std::string& name, const std::string& value)
            : name_(name)
            , value_(value)
        {
        
        }

        Property(std::string&& name, std::string&& value)
            : name_(std::move(name))
            , value_(std::move(value))
        {
        
        }

        Property& operator=(const Property& property)
        {
            name_ = property.name_;
            value_ = property.value_;
            propagate_ = property.propagate_;
            return *this;
        }

        Property& operator=(Property&& property)
        {
            name_ = std::move(property.name_);
            value_ = std::move(property.value_);
            propagate_ = property.propagate_;
            return *this;
        }

        bool operator==(const Property& other) const
        {
            return (this->name_ == other.name_)
                && (this->value_ == other.value_);
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

        void value(const std::string& value)
        {
            value_ = value;
        }

        void value(std::string&& value)
        {
            value_ = std::move(value);
        }

        const std::string& value() const
        {
            return value_;
        }

        std::string& value() 
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
        std::string value_;
        bool propagte_ = false;
};

using PropertySequence = std::vector<Property>;

class PropertyHelper
{
    public:
        static size_t serialized_size(const Property& property,
                size_t current_alignment = 0)
        {
            if (property.propagate())
            {
                size_t initial_alignment = current_alignment;
                current_alignment += 4 + alignment(current_alignment, 4)
                    + property.name().size() + 1;
                current_alignment += 4 + alignment(current_alignment, 4)
                    + property.value().size() + 1;

                return current_alignment - initial_alignment;
            }
            else
            {
                return 0;
            }
        }

        static size_t seriaized_size(const PropertySequence& properties,
                size_t current_alignment = 0)
        {
            size_t initial_alignment = current_alignment;
            current_alignment += 4 + alignment(current_alignment, 4);

            for (auto property = properties.begin(); 
                    property != properties.end(); ++property)
            {
                current_alignment += 
                    serialized_size(*property, current_alignment);
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

