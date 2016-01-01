#ifndef SN_CORELIB_NETWORK_RPC_API_INVOKER_H
#define SN_CORELIB_NETWORK_RPC_API_INVOKER_H

#include <QObject>
#include <QSharedPointer>
#include <QTcpSocket>
#include <QAtomicInt>
#include <QPair>
#include <QMap>
#include <QThread>
#include <QMutex>
#include <QByteArray>

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
   friend class TcpSocketDataDispatchWorker;
public:
   enum class ErrorType{
      None,
      ServerOffline,
      ConnectError
   };
public:
   using RequestCallbackType = void (*)(ApiInvokeResponse&, void*);
   using CallbackUnitType = QPair<RequestCallbackType, void*>;
public:
   ApiInvoker(const QString &host, quint16 port);
   ~ApiInvoker();
   bool request(ApiInvokeRequest& request, RequestCallbackType callback = nullptr, void* callbackArgs = nullptr);
   ApiInvokeResponse requestSync(const ApiInvokeRequest& request);
   void connectToServer();
   void disconnectFromServer();
   bool getStatus();
   ErrorType getError();
   QString& getErrorString();
protected:
   void writeRequestToSocket(const ApiInvokeRequest &request);
   void unboxResponse(const QByteArray &boxedRequest, ApiInvokeResponse &response);
   void resetStatus();
signals:
   void beginListenTcpSocketSignal();
   void connectedToServerSignal();
   void serverOfflineSignal();
   void requestSendBufferReady();
   void connectErrorSignal(ErrorType error, const QString &errorString);
public slots:
   void responseDataReceivedHandler();
   void serverOfflineHandler();
   void connectErrorHandler(QAbstractSocket::SocketError error, const QString &errorString);
protected:
   QMap<int, CallbackUnitType> m_callbackPool;
   static QAtomicInt sm_serial;
   QThread m_dataDispatchThread;
   QByteArray m_sendBuffer;
   QByteArray m_receiveBuffer;
   QByteArray m_packageUnitBuffer;
   QString m_host;
   quint16 m_port;
   QMutex m_sendBufferMutex;
   QMutex m_receiveBufferMutex;
   bool m_status;
   ErrorType m_error;
   QString m_errorString;
};

}//network
}//corelib
}//sn

#endif // SN_CORELIB_NETWORK_RPC_API_INVOKER_H
