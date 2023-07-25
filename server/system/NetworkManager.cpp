#include "system/NetworkManager.h"

namespace cc
{
    NetworkManager::NetworkManager()
        : _isRun(false)
    {

    }

    NetworkManager::~NetworkManager()
    {
        if (_isRun) {
            _isRun = false;
            _thread.join();
        }
    }

    int32_t NetworkManager::initialize()
    {
        if (_thread.isRunning() || _isRun) {
            return -1;
        }

        _isRun = true;

        _thread.start(*this);

        return 0;
    }

    void NetworkManager::run()
    {
        while (_isRun) {
            printf("<%s:%s(%d)> ** Is running..\n", __FILE__, __FUNCTION__, __LINE__);

            _thread.trySleep(1000);
        }
    }
}   // namespace cc