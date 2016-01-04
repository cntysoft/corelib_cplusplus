#include <QDataStream>

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

ApiInvokeRequest& ApiInvokeRequest::setSerial(int serial)
{
   m_serial = serial;
   return *this;
}

ApiInvokeRequest& ApiInvokeRequest::setSocketNum(int num)
{
   m_socketNum = num;
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

int ApiInvokeRequest::getSocketNum()const
{
   return m_socketNum;
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

ApiInvokeResponse& ApiInvokeResponse::setIsFinal(bool flag)
{
   m_isFinal = flag;
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

ApiInvokeResponse& ApiInvokeResponse::setData(const QMap<QString, QVariant> &data)
{
   m_data = data;
   return *this;
}

ApiInvokeResponse& ApiInvokeResponse::setExtraData(const QByteArray &extraData)
{
   m_extraData = extraData;
   return *this;
}

ApiInvokeResponse& ApiInvokeResponse::setError(const QPair<int, QString> &error)
{
   m_error = error;
   return *this;
}

const QString& ApiInvokeResponse::getSignature()const
{
   return m_signature;
}

int ApiInvokeResponse::getSerial() const
{
   return m_serial;
}

bool ApiInvokeResponse::isFinal()const
{
   return m_isFinal;
}

bool ApiInvokeResponse::getStatus()const
{
   return m_status;
}

const QMap<QString, QVariant>& ApiInvokeResponse::getData()const
{
   return m_data;
}

const QByteArray& ApiInvokeResponse::getExtraData()const
{
   return m_extraData;
}

const QPair<int, QString>& ApiInvokeResponse::getError()const
{
   return m_error;
}

QDataStream &operator<<(QDataStream &outStream, const ApiInvokeResponse &response)
{
   outStream << response.getSignature();
   outStream << (quint32)response.getSerial();
   outStream << response.isFinal();
   bool status = response.getStatus();
   outStream << status;
   if(status){
      bool hasData = false;
      const QMap<QString, QVariant>& data = response.getData();
      if(!data.empty()){
         hasData = true;
      }
      outStream << hasData;
      if(hasData){
         outStream << response.getData();
      }
   }else{
      //出错必须有出错信息
      outStream << response.getError();
   }
   bool hasExtraData = false;
   const QByteArray& extraData = response.getExtraData();
   if(!extraData.isEmpty()){
      hasExtraData = true;
   }
   outStream << hasExtraData;
   if(hasExtraData){
      outStream << extraData;
   }
   return outStream;
}

QDataStream &operator>>(QDataStream &inStream, ApiInvokeResponse &response)
{
   QString signature;
   quint32 serial;
   bool isFinal;
   inStream >> signature;
   inStream >> serial;
   inStream >> isFinal;
   response.setSignature(signature);
   response.setSerial(serial);
   response.setIsFinal(isFinal);
   bool status;
   inStream >> status;
   response.setStatus(status);
   if(status){
      bool hasData;
      inStream >> hasData;
      if(hasData){
         QMap<QString, QVariant> data;
         inStream >> data;
         response.setData(data);
      }
   }else{
      QPair<int, QString> error;
      inStream >> error;
      response.setError(error);
   }
   bool hasExtraData;
   inStream >> hasExtraData;
   if(hasExtraData){
      QByteArray extraData;
      inStream >> extraData;
      response.setExtraData(extraData);
   }
   return inStream;
}

}//network
}//corelib
}//sn