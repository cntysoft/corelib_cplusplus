#ifndef SN_CORELIB_DB_SQL_PLATFORM_MYSQL_CREATE_TABLE_DECORATOR_H 
#define SN_CORELIB_DB_SQL_PLATFORM_MYSQL_CREATE_TABLE_DECORATOR_H

#include <QMap>
#include <QString>

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
   friend ProcessResultPointerType create_table_decorator_process_columns(
         AbstractSql *self,const Engine &engine, 
         ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
         QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
public:
   CreateTableDecorator(const QString &table, const QString &schema);
   CreateTableDecorator(const TableIdentifier &table = TableIdentifier());
private:
   QString normalizeColumnOption(const QString &name);
protected:
   QMap<int, int> getSqlInsertOffsets(const QString &sql);
protected:
   QMap<QString, int> m_columnOptionSortOrder;
};

}//mysql
}//platform
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_PLATFORM_MYSQL_CREATE_TABLE_DECORATOR_H
