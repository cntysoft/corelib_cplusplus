#include <QDataStream>
#include <QMetaObject>
#include <QMetaMethod>
#include <QList>
#include <QDebug>

#include "api_provider.h"
#include "abstract_api.h"
#include "invoke_meta.h"
#include "api_error_code.h"

#include <QDebug>

namespace sn{
namespace corelib{
namespace network{

ApiProvider* ApiProvider::sm_self = nullptr;

ApiProvider::ApiProvider()
{}

ApiProvider& ApiProvider::instance()
{
   if(nullptr == ApiProvider::sm_self){
      ApiProvider::sm_self = new ApiProvider;
   }
   return *ApiProvider::sm_self;
}

ApiProvider& ApiProvider::setUnderlineSocket(int index, QTcpSocket* socket)
{
   if(!m_socketPool.contains(index)){
      m_socketPool.insert(index, socket);
      //绑定处理函数
      QObject::connect(socket, &QTcpSocket::disconnected, this, &ApiProvider::socketDisconnectHandler);
   }
   return *this;
}

void ApiProvider::socketDisconnectHandler()
{
   QTcpSocket *sockect = qobject_cast<QTcpSocket*>(sender());
   int socketNum = (int)sockect->socketDescriptor();
   QMap<QString, AbstractApi*>::const_iterator it = m_apiPool.cbegin();
   while(it != m_apiPool.cend()){
      it.value()->notifySocketDisconnect(socketNum);
      it++;
   }
   if(!m_batchDisconnectMode){
      m_socketPool.remove(m_socketPool.key(sockect));
   }
}

ApiProvider& ApiProvider::addApiToPool(const QString &key, ApiInitializerType initializerFn)
{
   if(!m_apiIntializerPool.contains(key)){
      m_apiIntializerPool.insert(key, initializerFn);
   }
   return *this;
}

void ApiProvider::callApi(const ApiInvokeRequest &request)
{
   AbstractApi *api = nullptr;
   QString key(request.getName());
   if(!m_apiPool.contains(key) && !m_apiIntializerPool.contains(key)){
      ApiInvokeResponse response("system/error", false);
      response.setError({API_CLS_NOT_EXIST, "指定的API类不存在"});
      response.setSerial(request.getSerial());
      writeResponseToSocket(request.getSocketNum(), response);
      return;
   }else if(!m_apiPool.contains(key)){
      //初始化api对象
      ApiInitializerType initializer = m_apiIntializerPool.value(key);
      api = initializer(*this);
      m_apiPool[key] = api;
   }else{
      api = m_apiPool[key];
   }
   //判断函数是否存在
   const QMetaObject *metaObject = api->metaObject();
   QString method = QString("%1(ApiInvokeRequest)").arg(request.getMethod());
   if(-1 == metaObject->indexOfMethod(method.toLatin1())){
      ApiInvokeResponse response("system/error", false);
      response.setError({API_METHOD_NOT_EXIST, QString("API %1 中没有函数 %2 ").arg(request.getName(), method)});
      response.setSerial(request.getSerial());
      writeResponseToSocket(request.getSocketNum(), response);
      return;
   }
   ApiInvokeResponse response;
   if(!metaObject->invokeMethod(api, request.getMethod().toLatin1(), Qt::DirectConnection, Q_RETURN_ARG(ApiInvokeResponse, response), Q_ARG(ApiInvokeRequest, request))){
      response.setStatus(false);
      response.setError({API_INVOKE_METHOD_ERROR, "there is no such member or the parameters did not match"});
      response.setSerial(request.getSerial());
   }
   response.setIsFinal(true);
   writeResponseToSocket(request.getSocketNum(), response);
}

void ApiProvider::writeResponseToSocket(int socketIndex, const ApiInvokeResponse &response)
{
   if(!m_socketPool.contains(socketIndex)){
      //这里是否写入出错提示
      return;
   }
   QTcpSocket *socket = m_socketPool[socketIndex];
   if(!socket->isOpen()){
      return;
   }
   QDataStream out(socket);
   out.setVersion(QDataStream::Qt_5_5);
   out << response;
   socket->write("\r\n\t");
   socket->flush();
}

void ApiProvider::disconnectUnderlineSockets()
{
   if(m_socketPool.empty()){
      return;
   }
   m_batchDisconnectMode = true;
   QList<int> keys = m_socketPool.keys();
   QList<int>::const_iterator it = keys.begin();
   while(it != keys.cend()){
      QTcpSocket* socket = m_socketPool.take(*it);
      socket->close();
      m_socketPool.remove(*it);
      it++;
   }
   m_batchDisconnectMode = false;
}

ApiProvider::~ApiProvider()
{
   m_batchDisconnectMode = true;
   disconnectUnderlineSockets();
   m_batchDisconnectMode = false;
}

}//network
}//corelib
}//sn