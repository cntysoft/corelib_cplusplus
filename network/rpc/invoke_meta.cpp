#include <QDataStream>

#include "invoke_meta.h"

namespace sn{
namespace corelib{
namespace network{

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

ApiInvokeRequest& ApiInvokeRequest::setSerial(int serial)
{
   m_serial = serial;
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

const QString& ApiInvokeRequest::getName()const
{
   return m_name;
}

int ApiInvokeRequest::getSerial()const
{
   return m_serial;
}

const QString& ApiInvokeRequest::getMethod()const
{
   return m_method;
}

const QList<QVariant>& ApiInvokeRequest::getArgs()const
{
   return m_args;
}

const QByteArray& ApiInvokeRequest::getExtraData()const
{
   return m_extraData;
}

QDataStream &operator<<(QDataStream &outStream, const ApiInvokeRequest &request)
{
   outStream << request.getName();
   outStream << request.getMethod();
   outStream << (quint32)request.getSerial();
   bool hasArgs = false;
   const QList<QVariant>& args = request.getArgs();
   if(!args.empty()){
      hasArgs = true;
   }
   outStream << hasArgs;
   if(hasArgs){
      outStream << args;
   }
   const QByteArray& extraData = request.getExtraData();
   bool hasExtraData = false;
   if(!extraData.isEmpty()){
      hasExtraData = true;
   }
   outStream << hasExtraData;
   if(hasExtraData){
      outStream << extraData;
   }
   return outStream;
}

QDataStream &operator>>(QDataStream &inStream, ApiInvokeRequest &request)
{
   QString name;
   QString method;
   quint32 serial;
   inStream >> name >> method;
   inStream >> serial;
   request.setName(name);
   request.setMethod(method);
   request.setSerial(serial);
   bool hasArgs;
   inStream >> hasArgs;
   if(hasArgs){
      QList<QVariant> args;
      inStream >> args;
      request.setArgs(args);
   }
   bool hasExtraData;
   inStream >> hasExtraData;
   if(hasExtraData){
      QByteArray extraData;
      inStream >> extraData;
      request.setExtraData(extraData);
   }
   return inStream;
}

ApiInvokeResponse::ApiInvokeResponse(const QString &signature, bool status)
   : m_signature(signature),
     m_status(status)
{}

ApiInvokeResponse& ApiInvokeResponse::setSignature(const QString &signature)
{
   m_signature = signature;
   return *this;
}

ApiInvokeResponse& ApiInvokeResponse::setSerial(int serial)
{
   m_serial = serial;
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

int ApiInvokeResponse::getSerial()
{
   return m_serial;
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