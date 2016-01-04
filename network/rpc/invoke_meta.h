#ifndef SN_CORELIB_NETWORK_RPC_INVOKE_META_H
#define SN_CORELIB_NETWORK_RPC_INVOKE_META_H

#include <QString>
#include <QByteArray>
#include <QList>
#include <QVariant>
#include <QPair>

#include "global/global.h"

QT_BEGIN_NAMESPACE
class QDataStream;
QT_END_NAMESPACE

namespace sn{
namespace corelib{
namespace network{

class SN_CORELIB_EXPORT ApiInvokeRequest
{
public:
   ApiInvokeRequest();
   ApiInvokeRequest(const QString& name, const QString& method, const QList<QVariant>& args = QList<QVariant>());
   ApiInvokeRequest& setName(const QString& name);
   ApiInvokeRequest& setSerial(int serial);
   ApiInvokeRequest& setMethod(const QString& method);
   ApiInvokeRequest& setArgs(const QList<QVariant>& args);
   ApiInvokeRequest& setExtraData(const QByteArray& extraData);
   ApiInvokeRequest& setSocketNum(int num);
   
   const QString& getName() const;
   const QString& getMethod()const;
   int getSerial()const;
   int getSocketNum()const;
   const QList<QVariant>& getArgs()const;
   const QByteArray& getExtraData()const;
protected:
   QString m_name;
   QString m_method;
   QList<QVariant> m_args;
   QByteArray m_extraData;
   int m_serial;
   int m_socketNum;
};

SN_CORELIB_EXPORT QDataStream &operator<<(QDataStream &outS, const ApiInvokeRequest &request);
SN_CORELIB_EXPORT QDataStream &operator>>(QDataStream &inS, ApiInvokeRequest &request);

class SN_CORELIB_EXPORT ApiInvokeResponse
{
public:
   ApiInvokeResponse();
   ApiInvokeResponse(const QString &signature, bool status);
   ApiInvokeResponse& setSignature(const QString &signature);
   ApiInvokeResponse& setSerial(int serial);
   ApiInvokeResponse& setStatus(bool status);
   ApiInvokeResponse& setIsFinal(bool flag);
   ApiInvokeResponse& setData(const QMap<QString, QVariant> &data);
   ApiInvokeResponse& setExtraData(const QByteArray &extraData);
   ApiInvokeResponse& setError(const QPair<int, QString>& error);
   
   const QString& getSignature()const;
   int getSerial()const;
   bool getStatus()const;
   const QMap<QString, QVariant>& getData()const;
   bool isFinal()const;
   const QByteArray& getExtraData()const;
   const QPair<int, QString>& getError()const;
   
protected:
   QString m_signature;
   bool m_status = true;
   QMap<QString, QString> m_data;
   QPair<int, QString> m_error;
   QByteArray m_extraData;
   int m_serial = 0;
   int m_isFinal = true;
};

SN_CORELIB_EXPORT QDataStream &operator<<(QDataStream &outS, const ApiInvokeResponse &request);
SN_CORELIB_EXPORT QDataStream &operator>>(QDataStream &inS, ApiInvokeResponse &request);

}//network
}//corelib
}//sn

Q_DECLARE_METATYPE(sn::corelib::network::ApiInvokeRequest)
Q_DECLARE_METATYPE(sn::corelib::network::ApiInvokeResponse)

#endif // SN_CORELIB_NETWORK_RPC_INVOKE_META_H
