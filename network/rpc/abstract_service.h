#ifndef SN_CORELIB_NETWORK_RPC_ABSTRACT_SERVICE_H
#define SN_CORELIB_NETWORK_RPC_ABSTRACT_SERVICE_H

#include <QObject>
#include "global/global.h"
#include "network/rpc/service_provider.h"

namespace sn{
namespace corelib{
namespace network{

class SN_CORELIB_EXPORT AbstractService : public QObject
{
   Q_OBJECT
friend class ServiceProvider;
public:
   AbstractService(ServiceProvider &provider);
   virtual ~AbstractService();
protected:
   void writeInterResponse(const ServiceInvokeRequest &request, ServiceInvokeResponse &response);
protected:
   virtual void notifySocketDisconnect(int socketDescriptor);
protected:
   ServiceProvider& m_serviceProvider;
};

}//network
}//corelib
}//sn

#endif // SN_CORELIB_NETWORK_RPC_ABSTRACT_SERVICE_H
