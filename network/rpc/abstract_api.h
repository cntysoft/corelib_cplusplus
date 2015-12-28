#ifndef SN_CORELIB_NETWORK_RPC_ABSTRACT_API_H
#define SN_CORELIB_NETWORK_RPC_ABSTRACT_API_H

#include <QObject>
#include "global/global.h"
#include "network/rpc/api_provider.h"

namespace sn{
namespace corelib{
namespace network{

class SN_CORELIB_EXPORT AbstractApi : public QObject
{
   Q_OBJECT
public:
   AbstractApi(ApiProvider &provider);
};

}//network
}//corelib
}//sn

#endif // SN_CORELIB_NETWORK_RPC_ABSTRACT_API_H
