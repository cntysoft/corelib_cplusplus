#ifndef SN_CORELIB_GLOBAL_COMMON_FUNCS_H
#define SN_CORELIB_GLOBAL_COMMON_FUNCS_H

#include "global.h"

#include <type_traits>
#include <QVariant>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include "kernel/std_error_type.h"

QT_BEGIN_NAMESPACE
class QString;
class QVariant;
QT_END_NAMESPACE

namespace sn{
namespace corelib{

class Application;
class ErrorInfo;

SN_CORELIB_EXPORT QString get_current_user_home_dir();
SN_CORELIB_EXPORT Application* get_application_ref();
SN_CORELIB_EXPORT QString get_corelib_version();

//template<typename T1, typename T2>
//bool SN_CORELIB_EXPORT is_super_of()
//{
//   return InstanceTester<T1>::testSuper(T2());
//}

template<typename T1, typename T2>
bool SN_CORELIB_EXPORT instanceof(const T2&)
{
   return std::is_base_of<T1, T2>::value;
}

QString SN_CORELIB_EXPORT format_str(const QString &format, const QStringList &args = QStringList());

bool SN_CORELIB_EXPORT is_scalar(const QVariant &value);

template<typename T>
bool SN_CORELIB_EXPORT map_has_requires(const QMap<QString, T> &map, const QStringList &requires, QStringList &leak)
{
   bool isOk = true;
   QStringList::const_iterator rit = requires.cbegin();
   QStringList::const_iterator rendMarker = requires.cend();
   while(rit != rendMarker){
      if(!map.contains(*rit)){
         leak.append(*rit);
         isOk = false;
      }
      rit++;
   }
   return isOk;
}

void SN_CORELIB_EXPORT throw_exception(ErrorInfo errorInfo, const QString &context = QString());

void SN_CORELIB_EXPORT dump_mysql_table(const QString &user, const QString &password, 
                                        const QString &dbname, const QString &table, 
                                        const QString &targetDir);

template<typename T>
void ensure_map_has_fields(const QMap<QString, T> &map, const QStringList& requires)throw(ErrorInfo)
{
   QStringList leak;
   map_has_requires(map, requires, leak);
   if(!leak.isEmpty()){
      throw_exception(ErrorInfo(sn::corelib::StdErrorType::msg(SN_E_API_INVOKE_LEAK_ARGS, leak), SN_E_API_INVOKE_LEAK_ARGS), 
                      STD_EXCEPTION_CONTEXT);
   }
}

QString SN_CORELIB_EXPORT hmac_sha1(const QString &key, const QString &secret);

}//corelib
}//sn

#endif // SN_CORELIB_GLOBAL_COMMON_FUNCS_H