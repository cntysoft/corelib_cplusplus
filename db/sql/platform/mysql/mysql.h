#ifndef SN_CORELIB_DB_SQL_PLATFORM_MYSQL_MYSQL_H
#define SN_CORELIB_DB_SQL_PLATFORM_MYSQL_MYSQL_H

#include "global/global.h"
#include "db/sql/platform/abstract_platform.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{
namespace mysql{

class SN_CORELIB_EXPORT Mysql : public AbstractPlatform
{
public:
   Mysql();   
};

}//mysql
}//platform
}//sql
}//db
}//corelib
}//sn
#endif // SN_CORELIB_DB_SQL_PLATFORM_MYSQL_MYSQL_H
