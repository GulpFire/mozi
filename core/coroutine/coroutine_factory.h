#ifndef MOZI_CORE_COROUTINE_COROUTINE_FACTORY_H
#define MOZI_CORE_COROUTINE_COROUTINE_FACTORY_H

#include <mozi/core/coroutine/mz_coroutine.h>
#include <mozi/core/log/logger.h>
#include <mozi/core/communication/transport/dds/rtps/storage/history.h>

#include <memory>
#include <utility>

namespace mozi {
namespace core {
namespace coroutine {

class CoroutineFactory
{
    public:
        using CreateCoroutineFunc = std::function<std::function<void()>>;
        CreateCoroutineFunc create_coroutine;
        
        inline std::shared_ptr<History> GetHistory() const { return history_; }
        
        inline void setHistory(const std::shared_ptr<History>& history)
        {
            history_ = history;
        }
    private:
        std::shared_ptr<History> history_{nullptr};
};


 
} // coroutine
} // core
} // mozi

#endif
