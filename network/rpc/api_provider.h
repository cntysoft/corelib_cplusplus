#ifndef SN_CORELIB_NETWORK_RPC_ABSTRACT_API_PROVIDER_H
#define SN_CORELIB_NETWORK_RPC_ABSTRACT_API_PROVIDER_H

#include <QSharedPointer>
#include <QTcpSocket>

#include "global/global.h"

namespace sn{
namespace corelib{
namespace network{

class AbstractApi;
class ApiInvokeResponse;
class ApiInvokeRequest;

class SN_CORELIB_EXPORT ApiProvider
{
   Q_DISABLE_COPY(ApiProvider)
public:
   using ApiInitializerType = AbstractApi* (*)(ApiProvider&);
   using ApiInitializerPoolType = QMap<QString, ApiInitializerType>;
   using ApiPoolType = QMap<QString, AbstractApi*>;
public:
   ApiInvokeResponse callApi(const ApiInvokeRequest &request);
   ApiProvider& addApiToPool(const QString &key, ApiInitializerType initializerFn);
   ApiProvider& setUnderlineSocket(QTcpSocket *socket);
public:
   static ApiProvider& instance();
protected:
   ApiProvider();
   void writeResponse(const ApiInvokeResponse& response);
   void initResponseByRequest(const ApiInvokeRequest &request, ApiInvokeResponse &response);
protected:
   static ApiProvider *sm_self;
   ApiInitializerPoolType m_apiIntializerPool;
   ApiPoolType m_apiPool;
   QTcpSocket *m_socket;
};


}//network
}//corelib
}//sn

#endif // SN_CORELIB_NETWORK_RPC_ABSTRACT_API_PROVIDER_H
