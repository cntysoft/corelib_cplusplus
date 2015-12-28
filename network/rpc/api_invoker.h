#ifndef SN_CORELIB_NETWORK_RPC_API_INVOKER_H
#define SN_CORELIB_NETWORK_RPC_API_INVOKER_H

#include <QObject>
#include <QSharedPointer>
#include <QTcpSocket>
#include <QAtomicInt>
#include <QPair>
#include <QMap>

#include "invoke_meta.h"
#include "global/global.h"

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

namespace sn{
namespace corelib{
namespace network{

class SN_CORELIB_EXPORT ApiInvoker : public QObject
{
   Q_OBJECT
public:
   using RequestCallbackType = void (*)(ApiInvokeResponse&, void*);
   using CallbackUnitType = QPair<RequestCallbackType, void*>;
public:
   ApiInvoker(const QSharedPointer<QTcpSocket>& socket);
   ~ApiInvoker();
   bool request(ApiInvokeRequest& request, RequestCallbackType callback = nullptr, void* callbackArgs = nullptr);
   ApiInvokeResponse requestSync(const ApiInvokeRequest& request);
public slots:
   void responseArriveHandler();
protected:
   void writeRequestToSocket(const ApiInvokeRequest &request);
   void unboxResponse(const QByteArray &boxedRequest, ApiInvokeResponse &response);
protected:
   QSharedPointer<QTcpSocket> m_socket;
   QMap<int, CallbackUnitType> m_callbackPool;
   static QAtomicInt sm_serial;
};

}//network
}//corelib
}//sn

#endif // SN_CORELIB_NETWORK_RPC_API_INVOKER_H
