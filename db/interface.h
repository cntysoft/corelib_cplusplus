#ifndef SN_CORELIB_DB_SQL_INTERFACE_H
#define SN_CORELIB_DB_SQL_INTERFACE_H

#include <QString>
#include <QPair>

#include "global/global.h"

namespace sn{
namespace corelib{
namespace db{

class SN_CORELIB_EXPORT SqlInterface
{
public:
   QString getSqlString();
};

class SN_CORELIB_EXPORT PlatformDecoratorInterface
{
   
};

}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_INTERFACE_H

