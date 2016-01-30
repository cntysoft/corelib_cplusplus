#include <QSharedPointer>

#include "platform_types.h"
#include "mysql/create_table_decorator.h"
#include "mysql/select_decorator.h"
#include "db/sql/abstract_sql.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{

using sn::corelib::db::sql::AbstractSql;

Mysql::Mysql()
{
   setTypeDecorator("sn::corelib::db::sql::platform::mysql::CreateTableDecorator", 
                    QSharedPointer<AbstractSql>(new mysql::CreateTableDecorator));
   setTypeDecorator("sn::corelib::db::sql::platform::mysql::SelectDecorator", 
                    QSharedPointer<AbstractSql>(new mysql::SelectDecorator));
}

}//platform
}//sql
}//db
}//corelib
}//sn