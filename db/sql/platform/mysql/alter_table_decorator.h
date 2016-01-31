#ifndef SN_CORELIB_DB_SQL_PLATFORM_MYSQL_ALTER_TABLE_DECORATOR_H 
#define SN_CORELIB_DB_SQL_PLATFORM_MYSQL_ALTER_TABLE_DECORATOR_H

#include <QMap>
#include <QString>

#include "global/global.h"
#include "db/sql/ddl/alter_table.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{
namespace mysql{

using sn::corelib::db::sql::ddl::AlterTable;

class SN_CORELIB_EXPORT AlterTableDecorator : public AlterTable
{
public:
   friend ProcessResultPointerType alter_table_decorator_process_add_columns(
         AbstractSql *self,const Engine &engine, 
         ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
         QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType alter_table_decorator_process_change_columns(
         AbstractSql *self,const Engine &engine, 
         ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
         QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
public:
   AlterTableDecorator(const QString &table, const QString &schema);
   AlterTableDecorator(const TableIdentifier &table = TableIdentifier());
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


#endif // SN_CORELIB_DB_SQL_PLATFORM_MYSQL_ALTER_TABLE_DECORATOR_H
