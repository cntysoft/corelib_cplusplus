#ifndef SN_CORELIB_GLOBAL_COMMON_FUNCS_H
#define SN_CORELIB_GLOBAL_COMMON_FUNCS_H

#include <QCoreApplication>

#include "global.h"

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

namespace sn
{
namespace corelib
{

SN_CORELIB_EXPORT QString get_current_user_home_dir();
SN_CORELIB_EXPORT const QCoreApplication& get_core_application_ref();
SN_CORELIB_EXPORT QString get_corelib_version();
}//corelib
}//sn

#endif // SN_CORELIB_GLOBAL_COMMON_FUNCS_H