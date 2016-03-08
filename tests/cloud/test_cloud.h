#ifndef UPGRADE_TEST_CLOUD_H
#define UPGRADE_TEST_CLOUD_H

#include <QObject>
#include <QString>
#include <QSharedPointer>

#include "cloud/aliyun/dns/dns_resolve.h"

namespace corelibtest{
namespace cloud{

using sn::corelib::cloud::aliyun::dns::DnsResolve;

class TestCloud : public QObject
{
   Q_OBJECT
public:
   TestCloud();
private slots:
   void initTestCase();
   void testAddDomainRecord();
protected:
   QSharedPointer<DnsResolve> m_dnsResolve;
};

}//cloud
}//corelibtest

#endif // UPGRADE_TEST_CLOUD_H
