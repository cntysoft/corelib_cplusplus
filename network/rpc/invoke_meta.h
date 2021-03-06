#ifndef SN_CORELIB_NETWORK_RPC_INVOKE_META_H
#define SN_CORELIB_NETWORK_RPC_INVOKE_META_H

#include <QString>
#include <QByteArray>
#include <QList>
#include <QVariant>
#include <QPair>
#include <QMap>

#include "global/global.h"

QT_BEGIN_NAMESPACE
class QDataStream;
QT_END_NAMESPACE

namespace sn{
namespace corelib{
namespace network{

class SN_CORELIB_EXPORT ServiceInvokeRequest
{
public:
   ServiceInvokeRequest();
   ServiceInvokeRequest(const QString &name, const QString &method, const QMap<QString, QVariant> &args = QMap<QString, QVariant>());
   ServiceInvokeRequest& setName(const QString &name);
   ServiceInvokeRequest& setSerial(int serial);
   ServiceInvokeRequest& setMethod(const QString &method);
   ServiceInvokeRequest& setArgs(const QMap<QString, QVariant> &args);
   ServiceInvokeRequest& setArg(const QString &name, const QVariant &value);
   ServiceInvokeRequest& setExtraData(const QByteArray& extraData);
   ServiceInvokeRequest& setSocketNum(int num);
   ServiceInvokeRequest& setIsWebSocket(bool flag);
   
   const QString& getName() const;
   const QString& getMethod()const;
   int getSerial()const;
   int getSocketNum()const;
   const QMap<QString, QVariant>& getArgs()const;
   const QVariant getArg(const QString &name)const;
   const QByteArray& getExtraData()const;
   bool isWebSocket()const;
protected:
   QString m_name;
   QString m_method;
   QMap<QString, QVariant> m_args;
   QByteArray m_extraData;
   int m_serial;
   int m_socketNum;
   bool m_isWebsocket = false;
};

SN_CORELIB_EXPORT QDataStream &operator<<(QDataStream &outS, const ServiceInvokeRequest &request);
SN_CORELIB_EXPORT QDataStream &operator>>(QDataStream &inS, ServiceInvokeRequest &request);

class SN_CORELIB_EXPORT ServiceInvokeResponse
{
public:
   ServiceInvokeResponse();
   ServiceInvokeResponse(const QString &signature, bool status);
   ServiceInvokeResponse& setSignature(const QString &signature);
   ServiceInvokeResponse& setSerial(int serial);
   ServiceInvokeResponse& setStatus(bool status);
   ServiceInvokeResponse& setIsFinal(bool flag);
   ServiceInvokeResponse& setData(const QMap<QString, QVariant> &data);
   ServiceInvokeResponse& setDataItem(const QString &key, const QVariant &value);
   ServiceInvokeResponse& removeDataItem(const QString &key);
   ServiceInvokeResponse& setExtraData(const QByteArray &extraData);
   ServiceInvokeResponse& setError(const QPair<int, QString>& error);
   const QString& getSignature()const;
   int getSerial()const;
   bool getStatus()const;
   const QMap<QString, QVariant>& getData()const;
   const QVariant getDataItem(const QString &key)const;
   bool isFinal()const;
   const QByteArray& getExtraData()const;
   const QPair<int, QString>& getError()const;
   void toJson(QString &json)const;
protected:
   QString m_signature;
   bool m_status = true;
   QMap<QString, QVariant> m_data;
   QPair<int, QString> m_error;
   QByteArray m_extraData;
   int m_serial = 0;
   bool m_isFinal = true;
};

SN_CORELIB_EXPORT QDataStream &operator<<(QDataStream &outS, const ServiceInvokeResponse &request);
SN_CORELIB_EXPORT QDataStream &operator>>(QDataStream &inS, ServiceInvokeResponse &request);

}//network
}//corelib
}//sn

Q_DECLARE_METATYPE(sn::corelib::network::ServiceInvokeRequest)
Q_DECLARE_METATYPE(sn::corelib::network::ServiceInvokeResponse)

#endif // SN_CORELIB_NETWORK_RPC_INVOKE_META_H
