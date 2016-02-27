#ifndef SN_CORELIB_UTILS_VERSION_H
#define SN_CORELIB_UTILS_VERSION_H

#include "global/global.h"

namespace sn{
namespace corelib{
namespace utils{

class SN_CORELIB_EXPORT Version
{
public:
   enum class PadMode
   {
      NaN,
      Infinity
   };
public:
   Version(const QString &version, PadMode defaultMode = PadMode::NaN);
   
};

}//utils
}//corelib
}//sn

#endif // SN_CORELIB_UTILS_VERSION_H
