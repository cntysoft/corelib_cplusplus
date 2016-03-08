#include "dns_api_caller.h"

#include <QNetworkReply>
#include <QUrl>
#include <QByteArray>
#include <QDebug>
#include <QRegularExpression>

namespace sn{
namespace corelib{
namespace cloud{
namespace aliyun{
namespace dns{

const QString DnsApiCaller::API_ENTRY = "http://dns.aliyuncs.com";

DnsApiCaller::DnsApiCaller(const QString &accesseKeyId, const QString &accessKeySecret)
   : m_accessKeyId(accesseKeyId),
     m_accessKeySecret(accessKeySecret),
     m_networkAccessManager(new QNetworkAccessManager)
{
   connect(m_networkAccessManager.data(), &QNetworkAccessManager::finished, this, &DnsApiCaller::requestFinishedHandler);
}

QString DnsApiCaller::percentEncode(QString queryStr)
{
   QString encoded = QUrl::toPercentEncoding(queryStr);
   encoded.replace(QRegularExpression("\\+"), "%20")
         .replace(QRegularExpression("\\*"), "%2A")
         .replace(QRegularExpression("%7E", QRegularExpression::CaseInsensitiveOption), "~");
   return encoded;
}

void DnsApiCaller::requestFinishedHandler(QNetworkReply *reply)
{
   QByteArray data = reply->readAll();
   qDebug() << data;
}

}//dns
}//aliyun
}//coud
}//corelib
}//sn