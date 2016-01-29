#ifndef SN_CORELIB_DB_SQL_PLATFORM_PLATFORM_TYPES_H
#define SN_CORELIB_DB_SQL_PLATFORM_PLATFORM_TYPES_H

#include "global/global.h"
#include "db/sql/platform/abstract_platform.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{

class SN_CORELIB_EXPORT Mysql : public AbstractPlatform
{
public:
   Mysql();   
};

}//platform
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_PLATFORM_PLATFORM_TYPES_H
