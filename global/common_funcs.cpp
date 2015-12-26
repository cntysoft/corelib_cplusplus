#include <QString>
#include <pwd.h>
#include <unistd.h>

#include "common_funcs.h"
#include "kernel/application.h"

namespace sn{
namespace corelib{

QString get_current_user_home_dir()
{
   passwd* pwd = getpwuid(getuid());
   return QString(pwd->pw_dir);
}

Application* get_application_ref()
{
   return Application::instance();
}

QString get_corelib_version()
{
   return QString(SN_CORELIB_VERSION);
}

}//corelib
}//sn