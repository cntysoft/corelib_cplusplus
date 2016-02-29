#include <QDataStream>
#include <QObject>
#include <QSharedPointer>
#include <QMutexLocker>

#include "service_invoker.h"
#include "kernel/errorinfo.h"
#include "tcpsocket_data_dispatch_worker.h"
#include <QThread>
#include <QBuffer>
#include <QDebug>

#include <QTcpSocket>

namespace sn{
namespace corelib{
namespace network{
using sn::corelib::ErrorInfo;

ServiceInvoker::ServiceInvoker(const QString& host, quint16 port)
   : m_host(host), 
     m_port(port),
     m_status(false),
     m_error(ErrorType::ServerOffline),
     m_errorString("server is offline")
{
}

void ServiceInvoker::connectToServer()
{
   if(!m_dataDispatchThread.isRunning()){
      TcpSocketDataDispatchWorker *worker = new TcpSocketDataDispatchWorker(m_host, m_port, this);
      QObject::connect(this, &ServiceInvoker::beginListenTcpSocketSignal, worker, &TcpSocketDataDispatchWorker::beginListenSocket);
      QObject::connect(this, &ServiceInvoker::requestSendBufferReady, worker, &TcpSocketDataDispatchWorker::requestSendBufferReadyHandler);
      QObject::connect(&m_dataDispatchThread, &QThread::finished, worker, &TcpSocketDataDispatchWorker::deleteLater);
      QObject::connect(worker, SIGNAL(connected()), this, SIGNAL(connectedToServerSignal()), Qt::DirectConnection);
      QObject::connect(worker, SIGNAL(disconnected()), this, SLOT(serverOfflineHandler()), Qt::DirectConnection);
      QObject::connect(worker, SIGNAL(connectError(QAbstractSocket::SocketError, const QString&)), this, SLOT(connectErrorHandler(QAbstractSocket::SocketError, const QString&)), Qt::DirectConnection);
      QObject::connect(worker, &TcpSocketDataDispatchWorker::responseReceiveBufferReady, this, &ServiceInvoker::responseDataReceivedHandler, Qt::QueuedConnection);
      worker->moveToThread(&m_dataDispatchThread);
      m_dataDispatchThread.start();
      emit beginListenTcpSocketSignal();
   }
}

void ServiceInvoker::resetStatus()
{
   m_sendBuffer.clear();
   m_receiveBuffer.clear();
   m_status = false;
   m_error = ErrorType::ServerOffline;
   m_errorString = "server offline";
}

void ServiceInvoker::serverOfflineHandler()
{
   resetStatus();
   m_dataDispatchThread.exit(0);
   emit serverOfflineSignal();
}

void ServiceInvoker::connectErrorHandler(QAbstractSocket::SocketError error, const QString &errorString)
{
   m_error = ErrorType::ConnectError;
   m_status = false;
   m_errorString = errorString;
   if(error != QAbstractSocket::SocketError::RemoteHostClosedError){
      emit connectErrorSignal(m_errorString);
   }
}

void ServiceInvoker::disconnectFromServer()
{
   if(m_dataDispatchThread.isRunning()){
      m_dataDispatchThread.exit(0);
      resetStatus();
   }
}

void ServiceInvoker::request(ServiceInvokeRequest &request, RequestCallbackType callback, void *callbackArgs)
{
   int serial = ++ServiceInvoker::sm_serial;
   request.setSerial(serial);
   if(nullptr != callback){
      m_callbackPool.insert(serial, CallbackUnitType(callback, callbackArgs));
   }
   writeRequestToSocket(request);
}

void ServiceInvoker::responseDataReceivedHandler()
{
   QMutexLocker locker(&m_receiveBufferMutex);
   QBuffer buffer(&m_receiveBuffer);
   buffer.open(QIODevice::ReadOnly);
   char byte;
   while(!buffer.atEnd()){
      buffer.getChar(&byte);
      if('\r' == byte){
         if(buffer.bytesAvailable() >= 2){
            char forward1;
            char forward2;
            buffer.getChar(&forward1);
            buffer.getChar(&forward2);
            if('\n' == forward1 && '\t' == forward2){
               //解压当前的包
               QDataStream stream(m_packageUnitBuffer);
               ServiceInvokeResponse response;
               stream >> response;
               processResponse(response);
               emit responseArrived(response);
               m_packageUnitBuffer.clear();
            }else{
               m_packageUnitBuffer.append(byte);
               buffer.ungetChar(forward2);
               buffer.ungetChar(forward1);
            }
         }
      }else{
         m_packageUnitBuffer.append(byte);
      }
   }
   m_receiveBuffer.clear();
}

void ServiceInvoker::processResponse(const ServiceInvokeResponse &response)
{
   int slotNum = response.getSerial();
   //这里必须存在
   if(m_callbackPool.contains(slotNum)){
      CallbackUnitType callbackUint = m_callbackPool.value(slotNum);
      callbackUint.first(response, callbackUint.second);
      if(response.isFinal()){
         m_callbackPool.remove(slotNum);
      }
   }else{
      //出错处理
   }
}

void ServiceInvoker::writeRequestToSocket(const ServiceInvokeRequest &request)
{
   QMutexLocker locker(&m_sendBufferMutex);
   //这里需要锁保护吗？
   QBuffer buffer(&m_sendBuffer);
   buffer.open(QIODevice::ReadWrite|QIODevice::Truncate);
   QDataStream out(&buffer);
   out.setVersion(QDataStream::Qt_5_5);
   out << request;
   buffer.write("\r\n\t");
   buffer.close();
   locker.unlock();
   emit requestSendBufferReady();
}

bool ServiceInvoker::getStatus()
{
   return m_status;
}

ServiceInvoker::ErrorType ServiceInvoker::getError()
{
   return m_error;
}

QString& ServiceInvoker::getErrorString()
{
   return m_errorString;
}

ServiceInvoker::~ServiceInvoker()
{
   if(m_dataDispatchThread.isRunning()){
      m_dataDispatchThread.exit(0);
      while(m_dataDispatchThread.isRunning()){
         
      }
   }
}

QAtomicInt ServiceInvoker::sm_serial = 0;

}//network
}//corelib
}//sn