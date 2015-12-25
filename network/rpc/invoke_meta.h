#ifndef SN_CORELIB_NETWORK_RPC_INVOKE_META_H
#define SN_CORELIB_NETWORK_RPC_INVOKE_META_H

#include <QString>
#include <QByteArray>
#include <QList>
#include <QVariant>
#include <QPair>

#include "global/global.h"

namespace sn{
namespace network{

class SN_CORELIB_EXPORT ApiInvokeRequest
{
public:
   ApiInvokeRequest(const QString& name, const QString& method, const QList<QVariant>& args);
   ApiInvokeRequest& setName(const QString& name);
   ApiInvokeRequest& setMethod(const QString& method);
   ApiInvokeRequest& setArgs(const QList<QVariant>& args);
   ApiInvokeRequest& setExtraData(const QByteArray& extraData);
   
   QString& getName();
   QString& getMethod();
   QList<QVariant>& getArgs();
   QByteArray& getExtraData();
protected:
   QString m_name;
   QString m_method;
   QList<QVariant> m_args;
   QByteArray m_extraData;
};

class SN_CORELIB_EXPORT ApiInvokeResponse
{
   ApiInvokeResponse(const QString &signature, bool status);
   
   ApiInvokeResponse& setSignature(const QString &signature);
   ApiInvokeResponse& setStatus(bool status);
   ApiInvokeResponse& setData(const QMap<QString, QString> &data);
   ApiInvokeResponse& setExtraData(const QByteArray &extraData);
   ApiInvokeResponse& setError(const QPair<int, QString>& error);
   
   QString getSignature();
   bool getStatus();
   QMap<QString, QString>& getData();
   QByteArray& getExtraData();
   QPair<int, QString>& getError();
   
protected:
   QString m_signature;
   bool m_status;
   QMap<QString, QString> m_data;
   QPair<int, QString> m_error;
   QByteArray m_extraData;
};

}//network
}//sn

#endif // SN_CORELIB_NETWORK_RPC_INVOKE_META_H
