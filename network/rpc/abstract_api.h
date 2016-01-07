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
friend class ApiProvider;
public:
   AbstractApi(ApiProvider &provider);
protected:
   virtual void notifySocketDisconnect(int socketDescriptor);
protected:
   ApiProvider& m_apiProvider;
};

}//network
}//corelib
}//sn

#endif // SN_CORELIB_NETWORK_RPC_ABSTRACT_API_H
