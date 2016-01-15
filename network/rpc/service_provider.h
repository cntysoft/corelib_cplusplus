#ifndef SN_CORELIB_NETWORK_RPC_ABSTRACT_SERVICE_PROVIDER_H
#define SN_CORELIB_NETWORK_RPC_ABSTRACT_SERVICE_PROVIDER_H

#include <QSharedPointer>
#include <QMap>
#include <QObject>

#include "global/global.h"

QT_BEGIN_INCLUDE_NAMESPACE
class QTcpSocket;
class QWebSocket;
QT_END_INCLUDE_NAMESPACE

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
   ServiceProvider& setUnderlineSocket(int index, QWebSocket *socket);
   void disconnectUnderlineSockets();
public:
   static ServiceProvider& instance();
    ~ServiceProvider();
protected:
   ServiceProvider();
   void writeResponseToSocket(const ServiceInvokeRequest &request, const ServiceInvokeResponse &response);
   void initResponseByRequest(const ServiceInvokeRequest &request, ServiceInvokeResponse &response);
protected slots:
   void socketDisconnectHandler();
protected:
   static ServiceProvider *sm_self;
   ServiceInitializerPoolType m_serviceIntializerPool;
   ServicePoolType m_servicePool;
   QMap<int, QTcpSocket*> m_socketPool;
   QMap<int, QWebSocket*> m_websocketPool;
   bool m_batchDisconnectMode = false;
};


}//network
}//corelib
}//sn

#endif // SN_CORELIB_NETWORK_RPC_ABSTRACT_SERVICE_PROVIDER_H
