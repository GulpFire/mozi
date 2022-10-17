#ifndef MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_MESSAGE_MESSAGE_RECEIVER_H
#define MOZI_CORE_COMMUNICATION_TRANSPORT_DDS_RTPS_MESSAGE_MESSAGE_RECEIVER_H

#include <unordered_map>
#include <mutex>
#include <functional>

namespace mozi {
namespace core {
namespace communication {
namespace dds {

class RTPSParticipantImpl;
class Endpoint;
class RTPSWriter;
class RTPSReader;
struct SubmessageHeader_t;

class MessageReceiver
{
    public:
        MessageReceiver(RTPSParticipantImpl* participant,
                uint32_t rec_buf_size);
        virtual ~MessageReceiver();

        void processCDRMessage(const Locater_t& source_locator,
                const Locator_t& reception_locator,
                CDRMessage_t* msg);
        void associateEndpoint(Endpoint* to_add);
        void removeEndpoint(Endpoint* to_remove);

    private:
        std::mutex mtx_;
        std::vector<RTPSWriter*> associated_writers_;
        std::unordered_map<EntityId_t, std::vector<RTPSReader*>> associated_readers_;

        RTPSParticipantImpl* participant_;
        ProtocolVersion_t source_version_;
        VendorId_t source_verndor_id_;

};

} // dds
} // communication
} // core
} // mozi

#endif
