#ifndef CLOUD_ALIYUN_DNS_DNS_API_CALLER_H
#define CLOUD_ALIYUN_DNS_DNS_API_CALLER_H

#include <algorithm>

#include <QNetworkAccessManager>
#include <QSharedPointer>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>
#include <QMap>
#include <QVariant>
#include <QDateTime>
#include <QTimeZone>
#include <ctime>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

#include "global/global.h"
#include "global/common_funcs.h"

namespace sn{
namespace corelib{
namespace cloud{
namespace aliyun{
namespace dns{

using sn::corelib::hmac_sha1;

class SN_CORELIB_EXPORT DnsApiCaller : public QObject
{
public:
   const static QString API_ENTRY;
   using ExtraResponseParser = void (*)(QMap<QString, QVariant>&);
public:
   DnsApiCaller(const QString &accesseKeyId, const QString &accessKeySecret);
   template<typename CallBackFn>
   void callApi(const QMap<QString, QString> &params, CallBackFn callback = nullptr, ExtraResponseParser extraResponseParser = nullptr);
protected:
   QString percentEncode(QString queryStr);
protected:
   QString m_accessKeyId;
   QString m_accessKeySecret;
   QSharedPointer<QNetworkAccessManager> m_networkAccessManager;
   QMetaObject::Connection m_connection;
};

template<typename CallBackFn>
void DnsApiCaller::callApi(const QMap<QString, QString> &params, CallBackFn callback, ExtraResponseParser extraResponseParser)
{
   QMap<QString, QString> requestParams(params);
   requestParams.insert("Format", "JSON");
   requestParams.insert("Version", "2015-01-09");
   requestParams.insert("SignatureVersion", "1.0");
   requestParams.insert("AccessKeyId", m_accessKeyId);
   requestParams.insert("SignatureMethod", "HMAC-SHA1");
   QDateTime dateTime = QDateTime::currentDateTime().toUTC();
   requestParams.insert("Timestamp", dateTime.toString("yyyy-MM-ddTHH:mm:ssZ"));
   std::srand(dateTime.toTime_t());
   requestParams.insert("SignatureNonce", QString("%1").arg(std::rand()));
   QList<QString> keys = requestParams.keys();
   std::sort(keys.begin(), keys.end(), [](const QString &left, const QString &right)->bool{
      return left < right;
   });
   QString query("");
   QString arg("");
   int length = keys.size();
   if(!keys.isEmpty()){
      for(int i = 0; i < length; i++){
         arg += percentEncode(keys[i]) + '=' + percentEncode(requestParams[keys[i]]) + '&';
      }
      query = arg.mid(0, arg.size() - 1);
   }
   QString baseString = "POST&%2F&" + percentEncode(query);
   requestParams.insert("Signature", hmac_sha1(baseString, m_accessKeySecret+'&'));
   QMap<QString, QString>::const_iterator it = requestParams.cbegin();
   QMap<QString, QString>::const_iterator endMarker = requestParams.cend();
   query.clear();
   while(it != endMarker){
      query += it.key() + '=' + percentEncode(it.value()) + '&';
      it++;
   }
   query = query.mid(0, query.size() - 1);
   QNetworkRequest request(QUrl(DnsApiCaller::API_ENTRY));
   request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
   auto requestHandler = [&](QNetworkReply *reply)->void{
      QByteArray data = reply->readAll();
      QJsonParseError parseError;
      QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &parseError);
      if(parseError.error == QJsonParseError::NoError){
         QMap<QString, QVariant> response;
         QJsonObject rootObject = jsonDoc.object();
         QJsonObject::const_iterator it = rootObject.constBegin();
         QJsonObject::const_iterator endMarker = rootObject.constEnd();
         while(it != endMarker){
            response.insert(it.key(), it.value().toVariant());
            it++;
         }
         if(nullptr != extraResponseParser){
            extraResponseParser(response);
         }
         callback(response);
      }else{
         callback(QMap<QString, QVariant>({
                                            {"Code", "Json parse error"}
                                         }));
      }
      m_networkAccessManager->disconnect(m_connection);
   };
   m_connection = connect(m_networkAccessManager.data(), &QNetworkAccessManager::finished, this, 
                                                requestHandler);
   m_networkAccessManager->post(request, QByteArray().append(query));
}

}//dns
}//aliyun
}//coud
}//corelib
}//sn

#endif // CLOUD_ALIYUN_DNS_DNS_API_CALLER_H
