#ifndef SN_CORELIB_DB_SQL_PLATFORM_PLATFORM_H 
#define SN_CORELIB_DB_SQL_PLATFORM_PLATFORM_H

#include "db/sql/interface.h"
#include "global/global.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{

using sn::corelib::db::sql::PlatformDecoratorInterface;
using sn::corelib::db::sql::SqlInterface;
using sn::corelib::db::sql::PreparableSqlInterface;

class SN_CORELIB_EXPORT Platform : public PlatformDecoratorInterface, public PreparableSqlInterface, public SqlInterface
{
   
};

}//platform
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_PLATFORM_PLATFORM_H

