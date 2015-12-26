#ifndef SN_CORELIB_GLOBAL_COMMON_FUNCS_H
#define SN_CORELIB_GLOBAL_COMMON_FUNCS_H

#include "global.h"

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

namespace sn{
namespace corelib{

class Application;

SN_CORELIB_EXPORT QString get_current_user_home_dir();
SN_CORELIB_EXPORT Application* get_application_ref();
SN_CORELIB_EXPORT QString get_corelib_version();

}//corelib
}//sn

#endif // SN_CORELIB_GLOBAL_COMMON_FUNCS_H