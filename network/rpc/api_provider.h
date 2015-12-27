#ifndef SN_CORELIB_NETWORK_RPC_ABSTRACT_API_PROVIDER_H
#define SN_CORELIB_NETWORK_RPC_ABSTRACT_API_PROVIDER_H

#include <QTcpSocket>

#include "global/global.h"

namespace sn{
namespace network{

class SN_CORELIB_EXPORT ApiProvider
{
   Q_DISABLE_COPY(ApiProvider)
protected:
   ApiProvider();
public:
   static ApiProvider& instance();
protected:
   static ApiProvider* sm_self;
};


}//network
}//sn

#endif // SN_CORELIB_NETWORK_RPC_ABSTRACT_API_PROVIDER_H
