#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>

#include "invoke_meta.h"

namespace sn{
namespace corelib{
namespace network{

ServiceInvokeRequest::ServiceInvokeRequest()
{}

ServiceInvokeRequest::ServiceInvokeRequest(const QString &name, const QString &method, const QMap<QString, QVariant> &args)
   : m_name(name),
     m_method(method),
     m_args(args),
     m_socketNum(-1)
{}

ServiceInvokeRequest& ServiceInvokeRequest::setName(const QString &name)
{
   m_name = name;
   return *this;
}

ServiceInvokeRequest& ServiceInvokeRequest::setIsWebSocket(bool flag)
{
   m_isWebsocket = flag;
   return *this;
}

ServiceInvokeRequest& ServiceInvokeRequest::setSerial(int serial)
{
   m_serial = serial;
   return *this;
}

ServiceInvokeRequest& ServiceInvokeRequest::setSocketNum(int num)
{
   m_socketNum = num;
   return *this;
}

ServiceInvokeRequest& ServiceInvokeRequest::setMethod(const QString &method)
{
   m_method = method;
   return *this;
}

ServiceInvokeRequest& ServiceInvokeRequest::setArgs(const QMap<QString, QVariant> &args)
{
   m_args = args;
   return *this;
}

ServiceInvokeRequest& ServiceInvokeRequest::setArg(const QString &name, const QVariant &value)
{
   m_args.insert(name, value);
   return *this;
}

ServiceInvokeRequest& ServiceInvokeRequest::setExtraData(const QByteArray &extraData)
{
   m_extraData = extraData;
   return *this;
}

const QString& ServiceInvokeRequest::getName()const
{
   return m_name;
}

bool ServiceInvokeRequest::isWebSocket()const
{
   return m_isWebsocket;
}

int ServiceInvokeRequest::getSerial()const
{
   return m_serial;
}

int ServiceInvokeRequest::getSocketNum()const
{
   return m_socketNum;
}

const QString& ServiceInvokeRequest::getMethod()const
{
   return m_method;
}

const QMap<QString, QVariant>& ServiceInvokeRequest::getArgs()const
{
   return m_args;
}

const QVariant ServiceInvokeRequest::getArg(const QString &name)const
{
   if(m_args.contains(name)){
      return m_args.value(name);
   }
   return QVariant();
}

const QByteArray& ServiceInvokeRequest::getExtraData()const
{
   return m_extraData;
}

QDataStream &operator<<(QDataStream &outStream, const ServiceInvokeRequest &request)
{
   outStream << request.getName();
   outStream << request.isWebSocket();
   outStream << request.getMethod();
   outStream << (quint32)request.getSerial();
   bool hasArgs = false;
   const QMap<QString, QVariant>& args = request.getArgs();
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

QDataStream &operator>>(QDataStream &inStream, ServiceInvokeRequest &request)
{
   QString name;
   QString method;
   bool isWebSocket;
   quint32 serial;
   inStream >> name;
   inStream >> isWebSocket;
   inStream >> method;
   inStream >> serial;
   request.setName(name);
   request.setIsWebSocket(isWebSocket);
   request.setMethod(method);
   request.setSerial(serial);
   bool hasArgs;
   inStream >> hasArgs;
   if(hasArgs){
      QMap<QString, QVariant> args;
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

ServiceInvokeResponse::ServiceInvokeResponse()
{}

ServiceInvokeResponse::ServiceInvokeResponse(const QString &signature, bool status)
   : m_signature(signature),
     m_status(status)
{}

ServiceInvokeResponse& ServiceInvokeResponse::setSignature(const QString &signature)
{
   m_signature = signature;
   return *this;
}

ServiceInvokeResponse& ServiceInvokeResponse::setIsFinal(bool flag)
{
   m_isFinal = flag;
   return *this;
}

ServiceInvokeResponse& ServiceInvokeResponse::setSerial(int serial)
{
   m_serial = serial;
   return *this;
}

ServiceInvokeResponse& ServiceInvokeResponse::setStatus(bool status)
{
   m_status = status;
   return *this;
}

ServiceInvokeResponse& ServiceInvokeResponse::setData(const QMap<QString, QVariant> &data)
{
   m_data = data;
   return *this;
}

ServiceInvokeResponse& ServiceInvokeResponse::setDataItem(const QString &key, const QVariant &value)
{
   m_data.insert(key, value);
   return *this;
}

ServiceInvokeResponse& ServiceInvokeResponse::removeDataItem(const QString &key)
{
   m_data.remove(key);
   return *this;
}

ServiceInvokeResponse& ServiceInvokeResponse::setExtraData(const QByteArray &extraData)
{
   m_extraData = extraData;
   return *this;
}

ServiceInvokeResponse& ServiceInvokeResponse::setError(const QPair<int, QString> &error)
{
   m_error = error;
   return *this;
}

const QString& ServiceInvokeResponse::getSignature()const
{
   return m_signature;
}

int ServiceInvokeResponse::getSerial() const
{
   return m_serial;
}

bool ServiceInvokeResponse::isFinal()const
{
   return m_isFinal;
}

bool ServiceInvokeResponse::getStatus()const
{
   return m_status;
}

const QMap<QString, QVariant>& ServiceInvokeResponse::getData()const
{
   return m_data;
}

const QVariant ServiceInvokeResponse::getDataItem(const QString &key)const
{
   if(m_data.contains(key)){
      return m_data.value(key);
   }
   return QVariant();
}

const QByteArray& ServiceInvokeResponse::getExtraData()const
{
   return m_extraData;
}

const QPair<int, QString>& ServiceInvokeResponse::getError()const
{
   return m_error;
}

void ServiceInvokeResponse::toJson(QString &json) const
{
   QJsonDocument jsonDoc;
   QJsonObject rootObject;
   rootObject.insert("signature", m_signature);
   rootObject.insert("status", m_status);
   rootObject.insert("serial", m_serial);
   rootObject.insert("final", m_isFinal);
   if(m_status){
      if(!m_data.isEmpty()){
         QJsonObject data;
         QMap<QString, QVariant>::const_iterator it = m_data.cbegin();
         while(it != m_data.cend()){
            data.insert(it.key(), QJsonValue::fromVariant(it.value()));
            it++;
         }
         rootObject.insert("data", data);
      }
   }else{
      rootObject.insert("errorCode", m_error.first);
      rootObject.insert("errorString", m_error.second);
   }
   if(!m_extraData.isEmpty()){
      rootObject.insert("extraData", QJsonValue(QString(m_extraData.toBase64())));
   }
   jsonDoc.setObject(rootObject);
   json.append(jsonDoc.toJson());
}

QDataStream &operator<<(QDataStream &outStream, const ServiceInvokeResponse &response)
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

QDataStream &operator>>(QDataStream &inStream, ServiceInvokeResponse &response)
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