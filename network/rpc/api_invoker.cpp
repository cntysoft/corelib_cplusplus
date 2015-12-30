#include <QDataStream>
#include <QObject>
#include <QDebug>
#include <QSharedPointer>
#include <QMutexLocker>

#include "api_invoker.h"
#include "kernel/errorinfo.h"
#include "tcpsocket_data_dispatch_worker.h"
#include <QThread>
#include <QBuffer>

#include <QTcpSocket>

namespace sn{
namespace corelib{
namespace network{
using sn::corelib::ErrorInfo;

ApiInvoker::ApiInvoker(const QString& host, quint16 port)
   : m_host(host), 
     m_port(port)
{
}

void ApiInvoker::connectToServer()
{
   if(!m_dataDispatchThread.isRunning()){
      TcpSocketDataDispatchWorker *worker = new TcpSocketDataDispatchWorker(m_host, m_port, this);
      QObject::connect(this, &ApiInvoker::beginListenTcpSocketSignal, worker, &TcpSocketDataDispatchWorker::beginListenSocket);
      QObject::connect(this, &ApiInvoker::requestSendBufferReady, worker, &TcpSocketDataDispatchWorker::requestSendBufferReadyHandler);
      QObject::connect(&m_dataDispatchThread, &QThread::finished, worker, &TcpSocketDataDispatchWorker::deleteLater);
      QObject::connect(worker, SIGNAL(connected()), this, SIGNAL(connectedToServerSignal()), Qt::DirectConnection);
      QObject::connect(worker, &TcpSocketDataDispatchWorker::responseReceiveBufferReady, this, &ApiInvoker::responseDataReceivedHandler, Qt::DirectConnection);
      worker->moveToThread(&m_dataDispatchThread);
      m_dataDispatchThread.start();
      emit beginListenTcpSocketSignal();
   }
}

void ApiInvoker::disconnectFromServer()
{
   m_dataDispatchThread.exit(0);
}

bool ApiInvoker::request(ApiInvokeRequest &request, RequestCallbackType callback, void *callbackArgs)
{
   int serial = ++ApiInvoker::sm_serial;
   request.setSerial(serial);
   m_callbackPool.insert(serial, CallbackUnitType(callback, callbackArgs));
   writeRequestToSocket(request);
   return true;
}

void ApiInvoker::responseDataReceivedHandler()
{
   QMutexLocker locker(&m_receiveBufferMutex);
   QBuffer buffer(&m_receiveBuffer);
   buffer.open(QIODevice::ReadOnly);
   char byte;
   while(!buffer.atEnd()){
      buffer.read(&byte, 1);
      if('\r' == byte){
         char forward;
         if(buffer.peek(&forward, 1) && '\n' == forward){
            //解压当前的包
            QDataStream stream(m_packageUnitBuffer);
            ApiInvokeResponse response;
            stream >> response;
            qDebug() << "package";
            //            qDebug() << response.getSignature();
            //            processRequest(request);
            m_packageUnitBuffer.clear();
            buffer.read(&forward, 1);
            continue;
         }
      }
      m_packageUnitBuffer.append(byte);
   }
   m_receiveBuffer.clear();
}

void ApiInvoker::writeRequestToSocket(const ApiInvokeRequest &request)
{
   QMutexLocker locker(&m_sendBufferMutex);
   //这里需要锁保护吗？
   QBuffer buffer(&m_sendBuffer);
   buffer.open(QIODevice::WriteOnly|QIODevice::Truncate);
   QDataStream out(&buffer);
   out.setVersion(QDataStream::Qt_5_5);
   out << request;
   buffer.write("\r\n");
   buffer.close();
   locker.unlock();
   emit requestSendBufferReady();
}

ApiInvoker::~ApiInvoker()
{
   //m_dataDispatchThread.exit(0);
   qDebug() << "destroy";
   //QObject::disconnect(m_socket.data(), &QTcpSocket::readyRead, this, &ApiInvoker::responseArriveHandler);
}

QAtomicInt ApiInvoker::sm_serial = 0;

}//network
}//corelib
}//sn