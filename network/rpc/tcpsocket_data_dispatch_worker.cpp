#include "tcpsocket_data_dispatch_worker.h"
#include "api_invoker.h"
#include <QByteArray>
#include <QBuffer>

namespace sn{
namespace corelib{
namespace network{

TcpSocketDataDispatchWorker::TcpSocketDataDispatchWorker(const QString &host, quint16 port, ApiInvoker *invoker)
   : m_host(host),
     m_port(port),
     m_apiInvoker(invoker)
{
}

void TcpSocketDataDispatchWorker::beginListenSocket()
{
   m_socket.reset(new QTcpSocket);
   connect(m_socket.data(), &QTcpSocket::connected, this, [&](){
      emit connected();
   }, Qt::QueuedConnection);
   connect(m_socket.data(), &QTcpSocket::readyRead, this, &TcpSocketDataDispatchWorker::responseDataProcessHandler, Qt::QueuedConnection);
   m_socket->connectToHost(m_host, m_port);
}

void TcpSocketDataDispatchWorker::requestSendBufferReadyHandler()
{
   QMutexLocker locker(&m_apiInvoker->m_sendBufferMutex);
   m_socket->write(m_apiInvoker->m_sendBuffer);
   m_socket->flush();
   m_apiInvoker->m_sendBuffer.clear();
}

void TcpSocketDataDispatchWorker::responseDataProcessHandler()
{
   //监听线程只负责投递数据， 不负责解析数据
   QMutexLocker locker(&m_apiInvoker->m_receiveBufferMutex);
   QBuffer buffer(&m_apiInvoker->m_receiveBuffer);
   buffer.open(QIODevice::WriteOnly|QIODevice::Append);
   buffer.write(m_socket->readAll());
   buffer.close();
   locker.unlock();
   emit responseReceiveBufferReady();
}

TcpSocketDataDispatchWorker::~TcpSocketDataDispatchWorker()
{
}

}//network
}//corelib
}//sn