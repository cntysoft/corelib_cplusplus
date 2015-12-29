#ifndef SN_CORELIB_NETWORK_RPC_ABSTRACT_API_PROVIDER_H
#define SN_CORELIB_NETWORK_RPC_ABSTRACT_API_PROVIDER_H

#include <QSharedPointer>
#include <QTcpSocket>
#include <QMap>
#include <QObject>

#include "global/global.h"

namespace sn{
namespace corelib{
namespace network{

class AbstractApi;
class ApiInvokeResponse;
class ApiInvokeRequest;

class SN_CORELIB_EXPORT ApiProvider : public QObject
{
   Q_DISABLE_COPY(ApiProvider)
   Q_OBJECT
public:
   using ApiInitializerType = AbstractApi* (*)(ApiProvider&);
   using ApiInitializerPoolType = QMap<QString, ApiInitializerType>;
   using ApiPoolType = QMap<QString, AbstractApi*>;
public:
   void callApi(const ApiInvokeRequest &request);
   ApiProvider& addApiToPool(const QString &key, ApiInitializerType initializerFn);
   ApiProvider& setUnderlineSocket(int index, QTcpSocket *socket);
public:
   static ApiProvider& instance();
protected:
   ApiProvider();
   void writeResponseToSocket(int socketIndex, const ApiInvokeResponse& response);
   void initResponseByRequest(const ApiInvokeRequest &request, ApiInvokeResponse &response);
protected slots:
   void socketDisconnectHandler();
protected:
   static ApiProvider *sm_self;
   ApiInitializerPoolType m_apiIntializerPool;
   ApiPoolType m_apiPool;
   QMap<int, QTcpSocket*> m_socketPool;
   QTcpSocket *m_socket;
};


}//network
}//corelib
}//sn

#endif // SN_CORELIB_NETWORK_RPC_ABSTRACT_API_PROVIDER_H
