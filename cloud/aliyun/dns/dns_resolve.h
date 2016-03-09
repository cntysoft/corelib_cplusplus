#ifndef CLOUD_ALIYUN_DNS_DNS_RESOLVE_H
#define CLOUD_ALIYUN_DNS_DNS_RESOLVE_H

#include <QSharedPointer>
#include <QString>
#include <QMap>
#include <QVariant>

#include "global/global.h"
#include "dns_api_caller.h"
#include "global/common_funcs.h"

namespace sn{
namespace corelib{
namespace cloud{
namespace aliyun{
namespace dns{

using sn::corelib::ensure_map_has_fields;

class SN_CORELIB_EXPORT DnsResolve
{
public:
   enum RecordType
   {
      A,//参考标准；RR值可为空，即@解析；不允许含有下划线；
      NS,//参考标准；RR值不能为空；允许含有下划线；不支持泛解析
      MX,//参考标准；RR值可为空，即@解析；不允许含有下划线
      TXT,//参考标准；另外，有效字符除字母、数字、“-”（中横杠）、还包括“_”(下划线)；RR值可为空，即@解析；允许含有下划线；不支持泛解析
      CNAME,//参考标准；另外，有效字符除字母、数字、“-”（中横杠）、还包括“_”(下划线)；RR值不允许为空（即@）；允许含有下划线
      SRV,//是一个name，且可含有下划线“_“和点“.”；允许含有下划线；可为空（即@）；不支持泛解析
      AAAA,//参考标准；RR值可为空，即@解析；不允许含有下划线；
      REDIRECT_URL,//参考标准；RR值可为空，即@解析
      FORWORD_URL//参考标准；RR值可为空，即@解析
   };
public:
   DnsResolve(QSharedPointer<DnsApiCaller> caller);
   template<typename CallbackFn>
   void addDomainRecord(const QString &domainName, const QString &key, 
                        RecordType type, const QString &value, CallbackFn callback = nullptr);
   template<typename CallbackFn>
   void deleteDomainRecord(const QString &recordId, CallbackFn callback = nullptr);
   template<typename CallbackFn>
   void modifyDomainRecord(const QString &recordId, const QString &key, 
                        RecordType type, const QString &value, CallbackFn callback = nullptr);
   template<typename CallbackFn>
   void setDomainRecordStatus(const QString &recordId, bool enabled, CallbackFn callback = nullptr);
   template<typename CallbackFn>
   void describeDomainRecords(const QString &domainName, int pageNum = 1, int pageSize = 500, CallbackFn callback = nullptr);
   template<typename CallbackFn>
   void describeDomainRecordInfo(const QString &recordId, CallbackFn callback = nullptr);
protected:
   QSharedPointer<DnsApiCaller> m_caller;
   const static QMap<RecordType, QString> sm_recordTypeMap;
};


template<typename CallbackFn>
void DnsResolve::addDomainRecord(const QString &domainName, const QString &key, 
                                 RecordType type, const QString &value, CallbackFn callback)
{
   QMap<QString, QString> params({
      {"Action", "AddDomainRecord"},
      {"DomainName", domainName},
      {"RR", key},
      {"Type", DnsResolve::sm_recordTypeMap[type]},
      {"Value", value}
   });
   m_caller->callApi(params, callback);
}

template<typename CallbackFn>
void DnsResolve::deleteDomainRecord(const QString &recordId, CallbackFn callback)
{
   QMap<QString, QString> params({
      {"Action", "DeleteDomainRecord"},
      {"RecordId", recordId}
   });
   m_caller->callApi(params, callback);
}

template<typename CallbackFn>
void DnsResolve::modifyDomainRecord(const QString &recordId, const QString &key, 
                     RecordType type, const QString &value, CallbackFn callback)
{
   QMap<QString, QString> params({
      {"Action", "UpdateDomainRecord"},
      {"RecordId", recordId},
      {"RR", key},
      {"Type", DnsResolve::sm_recordTypeMap[type]},
      {"Value", value}
   });
   m_caller->callApi(params, callback);
}

template<typename CallbackFn>
void DnsResolve::setDomainRecordStatus(const QString &recordId, bool enabled, CallbackFn callback)
{
   QMap<QString, QString> params({
      {"Action", "SetDomainRecordStatus"},
      {"RecordId", recordId},
      {"Status", enabled ? "Enable" : "Disable"}
   });
   m_caller->callApi(params, callback);
}

template<typename CallbackFn>
void DnsResolve::describeDomainRecords(const QString &domainName, int pageNum, int pageSize, CallbackFn callback)
{
   QMap<QString, QString> params({
      {"Action", "DescribeDomainRecords"},
      {"DomainName", domainName},
      {"PageNumber", QString("%1").arg(pageNum)},
      {"PageSize", QString("%1").arg(pageSize)}
   });
   m_caller->callApi(params, callback);
}

template<typename CallbackFn>
void DnsResolve::describeDomainRecordInfo(const QString &recordId, CallbackFn callback)
{
   QMap<QString, QString> params({
      {"Action", "DescribeDomainRecordInfo"},
      {"RecordId", recordId},
   });
   m_caller->callApi(params, callback);
}

}//dns
}//aliyun
}//coud
}//corelib
}//sn

#endif // CLOUD_ALIYUN_DNS_DNS_RESOLVE_H
