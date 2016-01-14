#ifndef SN_CORELIB_NETWORK_RPC_ABSTRACT_SERVICE_PROVIDER_H
#define SN_CORELIB_NETWORK_RPC_ABSTRACT_SERVICE_PROVIDER_H

#include <QSharedPointer>
#include <QTcpSocket>
#include <QMap>
#include <QObject>

#include "global/global.h"

namespace sn{
namespace corelib{
namespace network{

class AbstractService;
class ServiceInvokeResponse;
class ServiceInvokeRequest;

class SN_CORELIB_EXPORT ServiceProvider : public QObject
{
   Q_DISABLE_COPY(ServiceProvider)
   Q_OBJECT
   friend class AbstractService;
public:
   using ServiceInitializerType = AbstractService* (*)(ServiceProvider&);
   using ServiceInitializerPoolType = QMap<QString, ServiceInitializerType>;
   using ServicePoolType = QMap<QString, AbstractService*>;
public:
   void callService(const ServiceInvokeRequest &request);
   ServiceProvider& addServiceToPool(const QString &key, ServiceInitializerType initializerFn);
   ServiceProvider& setUnderlineSocket(int index, QTcpSocket *socket);
   void disconnectUnderlineSockets();
public:
   static ServiceProvider& instance();
    ~ServiceProvider();
protected:
   ServiceProvider();
   void writeResponseToSocket(int socketIndex, const ServiceInvokeResponse &response);
   void initResponseByRequest(const ServiceInvokeRequest &request, ServiceInvokeResponse &response);
protected slots:
   void socketDisconnectHandler();
protected:
   static ServiceProvider *sm_self;
   ServiceInitializerPoolType m_serviceIntializerPool;
   ServicePoolType m_servicePool;
   QMap<int, QTcpSocket*> m_socketPool;
   QTcpSocket *m_socket;
   bool m_batchDisconnectMode = false;
};


}//network
}//corelib
}//sn

#endif // SN_CORELIB_NETWORK_RPC_ABSTRACT_SERVICE_PROVIDER_H
