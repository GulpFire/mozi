#ifndef MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_STORAGE_CACHE_CHANGE_H
#define MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_STORAGE_CACHE_CHANGE_H

#include <mozi/core/time/time.h>

#include <atomic>
#include <cassert>

namespace mozi {
namespace core {
namespace communication {
namespace dds {

enum ChangeKind
{
    ALIVE,
    NOT_ALIVE_DISPOSED,
    NOT_ALIVE_UNREGISTERED,
    NOT_ALIVE_DISPOSED_UNREGISTERED
};

struct CacheChangeWriterInfo
{
    size_t num_size_submessage = 0;
    CacheChange* volatile prev = nullptr;
    CacheChange* volatile next = nullptr;
		std::atomic_bool is_linked{false};
};

struct CacheChangeReaderInfo
{
    Time reception_timestamp;
    int32_t disposed_generation_count;
    int32_t no_writers_generation_count;
		uint32_t writer_ownership_strength;
};

struct CacheChange
{
	ChangeKind kind = ALIVE;
	GUID writer_guid{};
	InstanceHandle instance_handle{};
	SequenceNumber sequence_number{};
	SerializedPayload serialized_payload{};
	SerializedPayload inline_qos{};

	bool is_read = false;

	TIme source_timestamp{};

	union
	{
		CacheChangeReaderInfo reader_info;
		CacheChangeWriterInfo writer_info;
	};

	WriteParams write_params{};
	bool is_untyped = true;

    CacheChange()
    {}

    CacheChange(const CacheChange&) = delete;
    const CacheChange& operator=(const CacheChange&) = delete;

    CacheChange(uint32_t payload_size, bool is_untyped = false)
        : serialized_payload(payload_size)
        , is_untyped_(is_untyped)
    {
    }

    bool copy(const CacheChange* change)
    {
        kind = change->kind;
        writerGUID = change->writerGUID;
        instance_handle = change->instance_handle;
        sequence_number = change->sequence_number;
        source_timestamp = change->source_timestamp;
        reader_info.reception_timestamp = 
            change->reader_info.reception_timestamp;
        write_params = change->write_params;
        is_read = change->is_read;
        fragment_size_ = change->fragment_size_;
        fragment_count_ = change->fragment_coumt_;
        first_missing_fragment_ = change->first_missing_fragment_;
        
        return serialized_payload.copy(&change->serialized_payload, 
                !change->is_untyped_)
    }

    void copyNotMemcpy(const CacheChange* change)
    {
        kind = change->kind;
        writerGUID = change->writerGUID;
        instance_handle = change->instance_handle;
        sequence_number = change->sequence_number;
        source_timestamp = change->source_timestamp;
        reader_info.reception_timestamp = 
            change->reader_info.reception_timestamp;
        writer_params = change->writer_params;
        is_read = change->is_read;

        serialized_payload.encapsulation = change->serialized_payload.encapsulation;

        setFragmentSize(change->fragment_size_, false);
    }

    ~CacheChange()
    {
        if (payload_owner_ != nullptr)
        {
            payload_owner_->releasePayload(*this);
        }
        assert(payload_owner_ == nullptr);
    }

    uint32_t getFragmentCount() const { return fragment_count_; }

    uint16_t getFragmentSize() const { return fragment_size_; }

    bool isFullyAssembled() 
    { 
        return first_missing_fragment_ >= fragment_count_; 
    }

    void getMissingFragments(FragmentNumberSet& frag_sns)
    {
        frag_sns.base(first_missing_fragment_ + 1);
        uint32_t current_frag = first_missing_fragment_;
        while (current_frag < fragment_count_)
        {
            frag_sns.add(current_frag + 1);
            current_frag = getNextMissingFragment(current_frag);
        }
    }

    void setFragmentSize(uint16_t fragment_size, 
            bool create_fragment_list = false)
    {
        fragment_size_ = fragment_size;
        fragment_count_ = 0;
        first_missing_fragment_ = 0;

        if (fragment_size > 0)
        {
            fragment_count_ = (serialized_payload.length + fragment_size - 1) 
                / fragment_size;
            if (create_fragment_list)
            {
                size_t offset = 0;
                for (uint32_t i = 1; i <= fragment_count_; 
                        i++, offset += fragment_size_)
                {
                    setNextMissingFragment(i - 1, i);
                }
            }
            else
            {
                first_missing_fragment_ = fragment_count_;
            }
        }
    }

    bool addFragments(const SerializedPayload& incoming_data,
            uint32_t fragment_starting_num, uint32_t fragments_in_submessage)
    {
        uint32_t original_offset = 
            (fragment_starting_num - 1) * fragment_size_;
        uint32_t incoming_length = fragment_size_ * fragments_in_submessage;
        uint32_t last_fragment_index = 
            fragment_starting_num + fragments_in_submessage - 1;

        if (last_fragment_index > fragment_count_)
        {
            return false;
        }

        if (last_fragment_index < fragment_count_)
        {
            if (incoming_data.length < incoming_length)
            {
                return false;
            }
        }
        else
        {
            incoming_length = serialized_payload.length - original_offset;
        }

        if (original_offset + incoming_length > serialized_payload.length)
        {
            return false;
        }

        if (received_fragments(fragment_starting_num - 1, 
                    fragments_in_submessage))
        {
            memcpy(&serialized_payload.data[original_offset],
                    incoming_data.data, incoming_length);
        }

        return isFullyAssembled();
    }

    PayloadPool const* payload_owner() const { return payload_owner_; } const;
    PayloadPool* payload_owner() { return payload_owner_; }

    void payload_owner(PayloadPool* owner) { payload_owner_ = owner; }
    private:
        uint16_t fragment_size_ = 0;
        uint32_t fragment_count_ = 0;
        uint32_t first_missing_fragment_ = 0;
        PayloadPool* payload_owner_ = nullptr;

        uint32_t getNextMissingFragment(uint32_t fragment_index)
        {
            uint32_t* ptr = nextFragmentPointer(fragment_index);
            return *ptr;
        }

        void setNextMissingFragment(uint32_t fragment_index, 
                uint32_t next_fragment_index)
        {
            uint32_t* ptr = nextFragmentPointer(fragment_index);
            *ptr = next_fragment_index;
        }
        
        uint32_t* nextFragmentPointer(uint32_t fragment_index)
        {
            size_t offset = fragment_size_;
            offset *= fragment_index;
            offset = (offset + 3u) & 3u;
            return reinterpret_cast<uint32_t*>(&serialized_payload.data[offset]);
        }

        bool receivedFragments(uint32_t initial_fragment, 
                uint32_t num_of_fragments)
        {
            bool at_leaset_one_changed = false;

            if ((fragment_size_ > 0) && (initial_fragment < fragment_count_))
            {
                uint32_t last_fragment = initial_fragment + num_of_fragments;
                if (last_fragment > fragment_count_)
                {
                    last_fragment = fragment_count_;
                }

                if (initial_fragment <= first_missing_fragment_)
                {
                    while (first_missing_fragment_ < last_fragment)
                    {
                        first_missing_fragment_ = 
                            getNextMissingFragment(first_missing_fragment_);
                        at_least_one_changed = true;
                    }
                }
                else
                {
                    uint32_t current_frag = first_missing_fragment_;
                    while (current_frag < initial_fragment)
                    {
                        uint32_t next_frag = get_next_missing_fragment(current_frag);
                        if (next_frag >= initail_fragment)
                        {
                            uint32_t next_missing_fragment = next_frag;
                            while (next_missing_fragment < last_fragment)
                            {
                                next_missing_fragment = getNextMissingFragment(next_missing_fragment);
                                at_least_one_changed = true;
                            }
                            if (at_least_one_changed)
                            {
                                setNextMissingFragment(current_flag, 
                                        next_missing_fragment);
                            }
                            break;
                        }
                        current_frag = next_frag;
                    }
                }
            }
            return at_least_one_changed;
        }

};

} // namespace dds
} // namespace communication
} // namespace core
} // namespace mozi

#endif
