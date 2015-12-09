#ifndef SN_CORELIB_STDDIR
#define SN_CORELIB_STDDIR

#include "global/global.h"

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

namespace sn 
{
namespace corelib 
{

class SN_CORELIB_STDDIR StdDir
{
public:
   static const QString& getShareResDir();
   static const QString& getAssetsDir();
   static const QString& getSysConfDir();
};

}//corelib
}//sn

#endif // SN_CORELIB_STDDIR

