#include "abstract_service.h"
#include "invoke_meta.h"

#include <QJsonDocument>
#include <QDebug>
#include <QTcpSocket>
#include <QWebSocket>

namespace sn{
namespace corelib{
namespace network{

AbstractService::AbstractService(ServiceProvider &provider)
   : m_serviceProvider(provider)
{}

void AbstractService::notifySocketDisconnect(QTcpSocket*)
{
}

void AbstractService::notifySocketDisconnect(QWebSocket*)
{
}

QSharedPointer<ServiceInvoker> AbstractService::getServiceInvoker(const QString &host, quint16 port)
{
   if(m_serviceInvoker.isNull()){
      m_serviceInvoker.reset(new ServiceInvoker(host, port));
   }
   return m_serviceInvoker;
}

void AbstractService::writeInterResponse(const ServiceInvokeRequest &request, ServiceInvokeResponse &response)
{
   response.setIsFinal(false);
   m_serviceProvider.writeResponseToSocket(request, response);
}

QByteArray AbstractService::encodeJsonObject(const QVariant &data)
{
   QJsonDocument doc = QJsonDocument::fromVariant(data);
   return doc.toJson();  
}

AbstractService::~AbstractService()
{
}

}//network
}//corelib
}//sn