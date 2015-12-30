#ifndef SN_CORELIB_NETWORK_RPC_TCPSOCKET_DATA_DISPATCH_WORKER_H
#define SN_CORELIB_NETWORK_RPC_TCPSOCKET_DATA_DISPATCH_WORKER_H

#include <QObject>
#include <QSharedPointer>
#include <QTcpSocket>
namespace sn{
namespace corelib{
namespace network{

class ApiInvoker;

class TcpSocketDataDispatchWorker : public QObject
{
   Q_OBJECT
public:
   TcpSocketDataDispatchWorker(const QString &host, quint16 port, ApiInvoker *apiInvoker);
   ~TcpSocketDataDispatchWorker();
signals:
   void connected();
   void responseReceiveBufferReady();
public slots:
   void beginListenSocket();
   void requestSendBufferReadyHandler();
   void responseDataProcessHandler();
protected:
   QSharedPointer<QTcpSocket> m_socket;
   QString m_host;
   quint16 m_port;
   ApiInvoker *m_apiInvoker;
};

}//network
}//corelib
}//sn

#endif // SN_CORELIB_NETWORK_RPC_TCPSOCKET_DATA_DISPATCH_WORKER_H
