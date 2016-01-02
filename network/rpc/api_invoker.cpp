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
     m_port(port),
     m_status(false),
     m_error(ErrorType::ServerOffline),
     m_errorString("server is offline")
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
      QObject::connect(worker, SIGNAL(disconnected()), this, SLOT(serverOfflineHandler()), Qt::DirectConnection);
      QObject::connect(worker, SIGNAL(connectError(QAbstractSocket::SocketError, const QString&)), this, SLOT(connectErrorHandler(QAbstractSocket::SocketError, const QString&)), Qt::DirectConnection);
      QObject::connect(worker, &TcpSocketDataDispatchWorker::responseReceiveBufferReady, this, &ApiInvoker::responseDataReceivedHandler, Qt::DirectConnection);
      worker->moveToThread(&m_dataDispatchThread);
      m_dataDispatchThread.start();
      emit beginListenTcpSocketSignal();
   }
}

void ApiInvoker::resetStatus()
{
   m_sendBuffer.clear();
   m_receiveBuffer.clear();
   m_status = false;
   m_error = ErrorType::ServerOffline;
   m_errorString = "server offline";
}

void ApiInvoker::serverOfflineHandler()
{
   resetStatus();
   m_dataDispatchThread.exit(0);
   emit serverOfflineSignal();
}

void ApiInvoker::connectErrorHandler(QAbstractSocket::SocketError error, const QString &errorString)
{
   m_error = ErrorType::ConnectError;
   m_status = false;
   m_errorString = errorString;
   if(error != QAbstractSocket::SocketError::RemoteHostClosedError){
      emit connectErrorSignal(m_error, m_errorString);
   }
}

void ApiInvoker::disconnectFromServer()
{
   if(m_dataDispatchThread.isRunning()){
      m_dataDispatchThread.exit(0);
      resetStatus();
   }
}

bool ApiInvoker::request(ApiInvokeRequest &request, RequestCallbackType callback, void *callbackArgs)
{
   int serial = ++ApiInvoker::sm_serial;
   request.setSerial(serial);
   if(nullptr != callback){
      m_callbackPool.insert(serial, CallbackUnitType(callback, callbackArgs));
   }
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
            //processRequest(response);
            m_packageUnitBuffer.clear();
            buffer.read(&forward, 1);
            continue;
         }
      }
      m_packageUnitBuffer.append(byte);
   }
   m_receiveBuffer.clear();
}

void ApiInvoker::processRequest(const ApiInvokeResponse &response)
{
   int slotNum = response.getSerial();
   //这里必须存在
   if(m_callbackPool.contains(slotNum)){
      CallbackUnitType callbackUint = m_callbackPool.value(slotNum);
      callbackUint.first(response, callbackUint.second);
   }else{
      //出错处理
   }
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

bool ApiInvoker::getStatus()
{
   return m_status;
}

ApiInvoker::ErrorType ApiInvoker::getError()
{
   return m_error;
}

QString& ApiInvoker::getErrorString()
{
   return m_errorString;
}

ApiInvoker::~ApiInvoker()
{
}

QAtomicInt ApiInvoker::sm_serial = 0;

}//network
}//corelib
}//sn