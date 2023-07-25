#include "CommonDefine.h"

#include "system/NetworkManager.h"

#include <memory>

std::unique_ptr<cc::NetworkManager> _pNetworkManager = nullptr;

int32_t main(int32_t argc, char* argv[])
{
    printf("<%s:%s(%d)> ** Circle Server Start..\n", __FILE__, __FUNCTION__, __LINE__);


    _pNetworkManager = std::make_unique<cc::NetworkManager>();

    if (_pNetworkManager == nullptr) {
        return -1;
    }

    _pNetworkManager->initialize();
    _pNetworkManager->join();
    

    return 0;
}