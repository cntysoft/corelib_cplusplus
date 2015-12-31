#include <QDataStream>

#include "api_provider.h"
#include "abstract_api.h"
#include "invoke_meta.h"

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
   m_socketPool.remove(m_socketPool.key(sockect));
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
   QString key(request.getName()+'/'+request.getMethod());
   if(!m_apiPool.contains(key) && !m_apiIntializerPool.contains(key)){
      ApiInvokeResponse response("system/error", false);
      response.setSerial(request.getSerial());
      writeResponseToSocket(request.getSocketNum(), response);
      return;
   }
}

void ApiProvider::writeResponseToSocket(int socketIndex, const ApiInvokeResponse &response)
{
   if(!m_socketPool.contains(socketIndex)){
      //这里是否写入出错提示
      return;
   }
   QTcpSocket *socket = m_socketPool[socketIndex];
   QDataStream out(socket);
   out.setVersion(QDataStream::Qt_5_5);
   out << response;
   socket->write("\r\n");
   socket->flush();
}

}//network
}//corelib
}//sn