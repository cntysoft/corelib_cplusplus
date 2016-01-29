#ifndef SN_CORELIB_DB_SQL_DDL_DROP_TABLE_H
#define SN_CORELIB_DB_SQL_DDL_DROP_TABLE_H

#include <QString>

#include "global/global.h"
#include "db/sql/abstract_sql.h"
#include "db/sql/table_identifier.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace ddl{

using sn::corelib::db::sql::AbstractSql;
using sn::corelib::db::sql::TableIdentifier;

class SN_CORELIB_EXPORT DropTable : public AbstractSql
{
public:
   const static QString TABLE;
   friend ProcessResultPointerType droptable_process_table(AbstractSql *self,const Engine &engine, 
                                                           ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                           QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
public:
   DropTable(const TableIdentifier &table = TableIdentifier());
   DropTable(const QString &table, const QString &schema = QString());
protected:
   TableIdentifier m_table;
};

}//ddl
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_DDL_DROP_TABLE_H

