#include "test_cloud.h"
#include <QtTest/QtTest>
#include <QDebug>

#include "cloud/aliyun/dns/dns_api_caller.h"

namespace corelibtest{
namespace cloud{

using sn::corelib::cloud::aliyun::dns::DnsApiCaller;

TestCloud::TestCloud()
{
   m_dnsResolve.reset(new DnsResolve(QSharedPointer<DnsApiCaller>(new DnsApiCaller("twJjKngFf6uySA0P", "3edbGFcrayDU4kaDrS004sp5J5Auod"))));
}
void TestCloud::initTestCase()
{
}

void TestCloud::testAddDomainRecord()
{
   m_dnsResolve->addDomainRecord("kelecloud.com", "abc", DnsResolve::A, "127.0.0.1", [&](const QMap<QString, QVariant> &response){
      qDebug() << response;
      m_eventLoop.quit();
   });
   m_eventLoop.exec();
}

void TestCloud::testDeleteDomainRecord()
{
   m_dnsResolve->deleteDomainRecord("77608330", [&](const QMap<QString, QVariant> &response){
      qDebug() << response;
      m_eventLoop.quit();
   });
   m_eventLoop.exec();
}

void TestCloud::testSetDomainRecordStatus()
{
   m_dnsResolve->setDomainRecordStatus("77609081", true, [&](const QMap<QString, QVariant> &response){
      qDebug() << response;
      m_eventLoop.quit();
   });
   m_eventLoop.exec();
}

void TestCloud::testDescribeDomainRecords()
{
   m_dnsResolve->describeDomainRecords("kelecloud.com", 1, 500, [&](const QMap<QString, QVariant> &response){
      qDebug() << response;
      m_eventLoop.quit();
   });
   m_eventLoop.exec();
}

}//cloud
}//corelibtest
QTEST_MAIN(corelibtest::cloud::TestCloud)