#include <QString>
#include <QDir>

#include "stddir.h"
#include "global/global.h"
#include "global/common_funcs.h"

namespace sn 
{
namespace corelib 
{

const QString& StdDir::getAssetsDir()
{
   static QString assetDir(StdDir::getShareResDir() + QDir::separator() + "assets");
   return assetDir;
}

const QString& StdDir::getShareResDir()
{
   static QString shareResDir(SN_SHARE_RES_DIR);
   return shareResDir;
}

const QString& StdDir::getSysConfDir()
{
   static QString cfgDir(get_current_user_home_dir() + "/.config/sheneninfo");
   return cfgDir;
}

}//corelib
}//sn
