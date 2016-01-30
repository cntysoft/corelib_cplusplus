#ifndef SN_CORELIB_DB_SQL_PLATFORM_MYSQL_SELECT_DECORATOR_H 
#define SN_CORELIB_DB_SQL_PLATFORM_MYSQL_SELECT_DECORATOR_H

#include "global/global.h"
#include "db/sql/select.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{
namespace mysql{

using sn::corelib::db::sql::Select;

class SN_CORELIB_EXPORT SelectDecorator : public Select
{
public:
   friend ProcessResultPointerType selectdecorator_process_limit(
         AbstractSql *self,const Engine &engine, 
         ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
         QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
public:
   SelectDecorator(const QString &table, const QString &schema);
   SelectDecorator(const TableIdentifier &table = TableIdentifier());
protected:
   virtual void localizeVariables();
};

}//mysql
}//platform
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_PLATFORM_MYSQL_SELECT_DECORATOR_H

