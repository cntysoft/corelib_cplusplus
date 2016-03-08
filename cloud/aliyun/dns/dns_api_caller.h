#ifndef CLOUD_ALIYUN_DNS_DNS_API_CALLER_H
#define CLOUD_ALIYUN_DNS_DNS_API_CALLER_H

#include <QNetworkAccessManager>
#include <QSharedPointer>
#include <QObject>
#include <QMap>
#include <QDateTime>

#include "global/global.h"

namespace sn{
namespace corelib{
namespace cloud{
namespace aliyun{
namespace dns{

class SN_CORELIB_EXPORT DnsApiCaller : public QObject
{
public:
   const static QString API_ENTRY;
public:
   DnsApiCaller(const QString &accesseKeyId, const QString &accessKeySecret);
   template<typename CallBackFn>
   void callApi(const QMap<QString, QString> &params, CallBackFn callback = nullptr);
protected slots:
   void requestFinishedHandler(QNetworkReply *reply);
protected:
   QString m_accessKeyId;
   QString m_accessKeySecret;
   QSharedPointer<QNetworkAccessManager> m_networkAccessManager;
};

template<typename CallBackFn>
void DnsApiCaller::callApi(const QMap<QString, QString> &params, CallBackFn callback)
{
   QMap<QString, QString> requestParams(params);
   requestParams.insert("Format", "JSON");
   requestParams.insert("Version", "2015-01-09");
   requestParams.insert("SignatureVersion", "1.0");
   requestParams.insert("AccessKeyId", m_accessKeyId);
   requestParams.insert("SignatureMethod", "HMAC-SHA1");
}

}//dns
}//aliyun
}//coud
}//corelib
}//sn

#endif // CLOUD_ALIYUN_DNS_DNS_API_CALLER_H
