#ifndef SN_CORELIB_NETWORK_RPC_SERVICE_INVOKER_H
#define SN_CORELIB_NETWORK_RPC_SERVICE_INVOKER_H

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

class SN_CORELIB_EXPORT ServiceInvoker : public QObject
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
   using RequestCallbackType = void (*)(const ServiceInvokeResponse&, void*);
   using CallbackUnitType = QPair<RequestCallbackType, void*>;
public:
   ServiceInvoker(const QString &host, quint16 port);
   ~ServiceInvoker();
   bool request(ServiceInvokeRequest& request, RequestCallbackType callback = nullptr, void* callbackArgs = nullptr);
   ServiceInvokeResponse requestSync(const ServiceInvokeRequest& request);
   void connectToServer();
   void disconnectFromServer();
   bool getStatus();
   ErrorType getError();
   QString& getErrorString();
protected:
   void writeRequestToSocket(const ServiceInvokeRequest &request);
   void unboxResponse(const QByteArray &boxedRequest, ServiceInvokeResponse &response);
   void processResponse(const ServiceInvokeResponse &response);
   void resetStatus();
signals:
   void beginListenTcpSocketSignal();
   void connectedToServerSignal();
   void serverOfflineSignal();
   void requestSendBufferReady();
   void connectErrorSignal(const QString &errorString);
   void responseArrived(const ServiceInvokeResponse &response);
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
Q_DECLARE_METATYPE(sn::corelib::network::ServiceInvoker::ErrorType)


#endif // SN_CORELIB_NETWORK_RPC_SERVICE_INVOKER_H
