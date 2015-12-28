#include "invoke_meta.h"

namespace sn{
namespace corelib{
namespace network{

ApiInvokeRequest::ApiInvokeRequest()
{}

ApiInvokeRequest::ApiInvokeRequest(const QString &name, const QString &method, const QList<QVariant> &args)
   : m_name(name),
     m_method(method),
     m_args(args)
{}

ApiInvokeRequest& ApiInvokeRequest::setName(const QString &name)
{
   m_name = name;
   return *this;
}

ApiInvokeRequest& ApiInvokeRequest::setMethod(const QString &method)
{
   m_method = method;
   return *this;
}

ApiInvokeRequest& ApiInvokeRequest::setArgs(const QList<QVariant> &args)
{
   m_args = args;
   return *this;
}

ApiInvokeRequest& ApiInvokeRequest::setExtraData(const QByteArray &extraData)
{
   m_extraData = extraData;
   return *this;
}

QString& ApiInvokeRequest::getName()
{
   return m_name;
}

QString& ApiInvokeRequest::getMethod()
{
   return m_method;
}

QList<QVariant>& ApiInvokeRequest::getArgs()
{
   return m_args;
}

QByteArray& ApiInvokeRequest::getExtraData()
{
   return m_extraData;
}

ApiInvokeResponse::ApiInvokeResponse()
{}

ApiInvokeResponse::ApiInvokeResponse(const QString &signature, bool status)
   : m_signature(signature),
     m_status(status)
{}

ApiInvokeResponse& ApiInvokeResponse::setSignature(const QString &signature)
{
   m_signature = signature;
   return *this;
}

ApiInvokeResponse& ApiInvokeResponse::setStatus(bool status)
{
   m_status = status;
   return *this;
}

ApiInvokeResponse& ApiInvokeResponse::setData(const QMap<QString, QString> &data)
{
   m_data = data;
   return *this;
}

ApiInvokeResponse& ApiInvokeResponse::setExtraData(const QByteArray &extraData)
{
   m_extraData = extraData;
   return *this;
}

QString& ApiInvokeResponse::getSignature()
{
   return m_signature;
}

bool ApiInvokeResponse::getStatus()
{
   return m_status;
}

QMap<QString, QString>& ApiInvokeResponse::getData()
{
   return m_data;
}

QByteArray& ApiInvokeResponse::getExtraData()
{
   return m_extraData;
}


}//network
}//corelib
}//sn