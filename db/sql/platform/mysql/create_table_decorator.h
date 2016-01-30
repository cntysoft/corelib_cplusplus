#ifndef SN_CORELIB_DB_SQL_PLATFORM_MYSQL_CREATE_TABLE_DECORATOR_H 
#define SN_CORELIB_DB_SQL_PLATFORM_MYSQL_CREATE_TABLE_DECORATOR_H

#include "global/global.h"
#include "db/sql/ddl/create_table.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{
namespace mysql{

using sn::corelib::db::sql::ddl::CreateTable;

class SN_CORELIB_EXPORT CreateTableDecorator : public CreateTable
{
public:
   
};

}//mysql
}//platform
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_PLATFORM_MYSQL_CREATE_TABLE_DECORATOR_H
