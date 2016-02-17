#include <QTcpSocket>
#include <QWebSocket>
#include <QDataStream>
#include <QMetaObject>
#include <QMetaMethod>
#include <QList>
#include <QString>
#include <QDebug>

#include "service_provider.h"
#include "abstract_service.h"
#include "invoke_meta.h"
#include "service_error_code.h"

#include <QDebug>

namespace sn{
namespace corelib{
namespace network{

ServiceProvider* ServiceProvider::sm_self = nullptr;

ServiceProvider::ServiceProvider()
{}

ServiceProvider& ServiceProvider::instance()
{
   if(nullptr == ServiceProvider::sm_self){
      ServiceProvider::sm_self = new ServiceProvider;
   }
   return *ServiceProvider::sm_self;
}

ServiceProvider& ServiceProvider::setUnderlineSocket(int index, QTcpSocket *socket)
{
   if(!m_socketPool.contains(index)){
      m_socketPool.insert(index, socket);
      //绑定处理函数
      QObject::connect(socket, &QTcpSocket::disconnected, this, &ServiceProvider::socketDisconnectHandler);
   }
   return *this;
}

ServiceProvider& ServiceProvider::setUnderlineSocket(int index, QWebSocket *socket)
{
   if(!m_websocketPool.contains(index)){
      m_websocketPool.insert(index, socket);
      //绑定处理函数
      QObject::connect(socket, &QWebSocket::disconnected, this, &ServiceProvider::socketDisconnectHandler);
   }
   return *this;
}

void ServiceProvider::socketDisconnectHandler()
{
   QTcpSocket *sockect = qobject_cast<QTcpSocket*>(sender());
   if(nullptr == sockect){
      return;
   }
   int socketNum = (int)sockect->socketDescriptor();
   QMap<QString, AbstractService*>::const_iterator it = m_servicePool.cbegin();
   while(it != m_servicePool.cend()){
      it.value()->notifySocketDisconnect(socketNum);
      it++;
   }
   if(!m_batchDisconnectMode){
      m_socketPool.remove(m_socketPool.key(sockect));
   }
}

ServiceProvider& ServiceProvider::addServiceToPool(const QString &key, ServiceInitializerType initializerFn)
{
   if(!m_serviceIntializerPool.contains(key)){
      m_serviceIntializerPool.insert(key, initializerFn);
   }
   return *this;
}

void ServiceProvider::callService(const ServiceInvokeRequest &request)
{
   AbstractService *service = nullptr;
   QString key(request.getName());
   if(!m_servicePool.contains(key) && !m_serviceIntializerPool.contains(key)){
      ServiceInvokeResponse response("system/error", false);
      response.setError({SERVICE_CLS_NOT_EXIST, "指定的SERVICE类不存在"});
      response.setSerial(request.getSerial());
      writeResponseToSocket(request, response);
      return;
   }else if(!m_servicePool.contains(key)){
      //初始化service对象
      ServiceInitializerType initializer = m_serviceIntializerPool.value(key);
      service = initializer(*this);
      m_servicePool[key] = service;
   }else{
      service = m_servicePool[key];
   }
   //判断函数是否存在
   const QMetaObject *metaObject = service->metaObject();
   QString method = QString("%1(ServiceInvokeRequest)").arg(request.getMethod());
   if(-1 == metaObject->indexOfMethod(method.toLatin1())){
      ServiceInvokeResponse response("system/error", false);
      response.setError({SERVICE_METHOD_NOT_EXIST, QString("SERVICE %1 中没有函数 %2 ").arg(request.getName(), method)});
      response.setSerial(request.getSerial());
      writeResponseToSocket(request, response);
      return;
   }
   ServiceInvokeResponse response;
   if(!metaObject->invokeMethod(service, request.getMethod().toLatin1(), Qt::DirectConnection, Q_RETURN_ARG(ServiceInvokeResponse, response), Q_ARG(ServiceInvokeRequest, request))){
      response.setStatus(false);
      response.setError({SERVICE_INVOKE_METHOD_ERROR, "there is no such member or the parameters did not match"});
      response.setSerial(request.getSerial());
   }
   response.setIsFinal(true);
   writeResponseToSocket(request, response);
}

void ServiceProvider::writeResponseToSocket(const ServiceInvokeRequest &request, const ServiceInvokeResponse &response)
{
   int index = request.getSocketNum();
   if(request.isWebSocket()){
      if(!m_websocketPool.contains(index)){
         //这里是否写入出错提示
         return;
      }
      QWebSocket *socket = m_websocketPool[index];
      if(!socket->isValid()){
         return;
      }
      QString package;
      response.toJson(package);
      socket->sendTextMessage(package);
      socket->flush();
   }else{
      if(!m_socketPool.contains(index)){
         //这里是否写入出错提示
         return;
      }
      QTcpSocket *socket = m_socketPool[index];
      if(!socket->isOpen()){
         return;
      }
      QDataStream out(socket);
      out.setVersion(QDataStream::Qt_5_5);
      out << response;
      socket->write("\r\n\t");
      socket->flush();
   }
}

void ServiceProvider::disconnectUnderlineSockets()
{
   if(m_socketPool.empty() && m_websocketPool.empty()){
      return;
   }
   m_batchDisconnectMode = true;
   if(!m_socketPool.isEmpty()){
      QList<int> keys = m_socketPool.keys();
      QList<int>::const_iterator it = keys.begin();
      while(it != keys.end()){
         QTcpSocket *socket = m_socketPool.take(*it);
         if(nullptr == socket){
            continue;
         }
         if(socket->isOpen()){
            socket->close();
         }
         m_socketPool.remove(*it);
         it++;
      }
   }
   if(!m_websocketPool.isEmpty()){
      QList<int> keys = m_websocketPool.keys();
      QList<int>::const_iterator it = keys.begin();
      while(it != keys.end()){
         QWebSocket *socket = m_websocketPool.take(*it);
         if(nullptr == socket){
            continue;
         }
         if(socket->state() != QAbstractSocket::UnconnectedState && socket->state() != QAbstractSocket::ClosingState){
            socket->close();
         }
         m_websocketPool.remove(*it);
         it++;
      }
   }
   m_batchDisconnectMode = false;
}

ServiceProvider::~ServiceProvider()
{
   m_batchDisconnectMode = true;
   disconnectUnderlineSockets();
   m_batchDisconnectMode = false;
   ServicePoolType::iterator it = m_servicePool.begin();
   ServicePoolType::iterator endMarker = m_servicePool.end();
   while(it != endMarker){
      delete it.value();
      it++;
   }
   m_servicePool.clear();
}

}//network
}//corelib
}//sn