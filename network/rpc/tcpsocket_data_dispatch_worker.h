#ifndef SN_CORELIB_NETWORK_RPC_TCPSOCKET_DATA_DISPATCH_WORKER_H
#define SN_CORELIB_NETWORK_RPC_TCPSOCKET_DATA_DISPATCH_WORKER_H

#include <QObject>
#include <QSharedPointer>
#include <QTcpSocket>
namespace sn{
namespace corelib{
namespace network{

class ServiceInvoker;

class TcpSocketDataDispatchWorker : public QObject
{
   Q_OBJECT
public:
   TcpSocketDataDispatchWorker(const QString &host, quint16 port, ServiceInvoker *serviceInvoker);
   ~TcpSocketDataDispatchWorker();
signals:
   void connected();
   void disconnected();
   void responseReceiveBufferReady();
   void connectError(QAbstractSocket::SocketError error, const QString& errorString);
public slots:
   void beginListenSocket();
   void requestSendBufferReadyHandler();
   void responseDataProcessHandler();
   void connectErrorHandler(QAbstractSocket::SocketError error);
protected:
   QSharedPointer<QTcpSocket> m_socket;
   QString m_host;
   quint16 m_port;
   ServiceInvoker *m_serviceInvoker;
};

}//network
}//corelib
}//sn

#endif // SN_CORELIB_NETWORK_RPC_TCPSOCKET_DATA_DISPATCH_WORKER_H
