#ifndef SN_CORELIB_GLOBAL_COMMON_FUNCS_H
#define SN_CORELIB_GLOBAL_COMMON_FUNCS_H
#include "global.h"

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

namespace sn
{
namespace corelib
{

SN_CORELIB_EXPORT QString get_current_user_home_dir();

}//corelib
}//sn

#endif // SN_CORELIB_GLOBAL_COMMON_FUNCS_H