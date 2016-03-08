#include "dns_resolve.h"

namespace sn{
namespace corelib{
namespace cloud{
namespace aliyun{
namespace dns{

DnsResolve::DnsResolve(QSharedPointer<DnsApiCaller> caller)
   : m_caller(caller)
{}

const QMap<DnsResolve::RecordType, QString> DnsResolve::sm_recordTypeMap{
   {DnsResolve::RecordType::A, "A"},
   {DnsResolve::RecordType::NS, "NS"},
   {DnsResolve::RecordType::MX, "MX"},
   {DnsResolve::RecordType::TXT, "TXT"},
   {DnsResolve::RecordType::CNAME, "CNAME"},
   {DnsResolve::RecordType::SRV, "SRV"},
   {DnsResolve::RecordType::AAAA, "AAAA"},
   {DnsResolve::RecordType::REDIRECT_URL, "REDIRECT_URL"},
   {DnsResolve::RecordType::FORWORD_URL, "FORWORD_URL"}
};

}//dns
}//aliyun
}//coud
}//corelib
}//sn