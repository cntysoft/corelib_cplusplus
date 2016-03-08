#include "dns_api_caller.h"

#include <QNetworkReply>

namespace sn{
namespace corelib{
namespace cloud{
namespace aliyun{
namespace dns{

DnsApiCaller::DnsApiCaller(const QString &accesseKeyId, const QString &accessKeySecret)
   : m_accessKeyId(accesseKeyId),
     m_accessKeySecret(accessKeySecret),
     m_networkAccessManager(new QNetworkAccessManager)
{
   connect(m_networkAccessManager.data(), &QNetworkAccessManager::finished, this, &DnsApiCaller::requestFinishedHandler);
}

void DnsApiCaller::requestFinishedHandler(QNetworkReply *reply)
{
   
}

}//dns
}//aliyun
}//coud
}//corelib
}//sn