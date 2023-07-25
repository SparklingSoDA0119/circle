#ifndef _CC_SYSTEM_NETWORKMANAGER_H_
#define _CC_SYSTEM_NETWORKMANAGER_H_

#include "CommonDefine.h"

#include <Poco/Thread.h>

namespace cc
{
    class NetworkManager : public Poco::Runnable
    {
    public :
        NetworkManager();
        virtual ~NetworkManager();

    public :
        int32_t initialize();

    public :
        void run() override;
        void join();

    private :
        bool _isRun;

        Poco::Thread _thread;

    };  // class NetworkManager


    inline void NetworkManager::join()
    {
        _thread.join();
    }
}   // namespace cc


#endif  // _CC_SYSTEM_NETWORKMANAGER_H_