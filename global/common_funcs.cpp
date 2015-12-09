#include <QCoreApplication>
#include <QString>
#include <pwd.h>
#include <unistd.h>

#include "common_funcs.h"

namespace sn 
{
namespace corelib 
{

QString get_current_user_home_dir()
{
   passwd* pwd = getpwuid(getuid());
   return QString(pwd->pw_dir);
}
}//corelib
}//sn