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
   m_dnsResolve->addDomainRecord("www.kelecloud.com", "sheneninfo", DnsResolve::A, "127.0.0.1", [](){
      
   });
}

void TestCloud::testAddDomainRecord()
{
   
}

}//cloud
}//corelibtest
QTEST_MAIN(corelibtest::cloud::TestCloud)