#ifndef SN_CORELIB_GLOBAL_COMMON_FUNCS_H
#define SN_CORELIB_GLOBAL_COMMON_FUNCS_H

#include "global.h"
#include <type_traits>
#include <QVariant>
#include <QString>

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

namespace sn{
namespace corelib{

class Application;

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

}//corelib
}//sn

#endif // SN_CORELIB_GLOBAL_COMMON_FUNCS_H