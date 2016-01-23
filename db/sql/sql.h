#ifndef SN_CORELIB_DB_SQL_SQL_H
#define SN_CORELIB_DB_SQL_SQL_H

#include <QSharedPointer>

#include "table_identifier.h"
#include "global/global.h"
#include "db/engine/engine.h"
#include "db/sql/platform/platform.h"
#include "db/sql/delete.h"
#include "db/sql/abstract_sql.h"
#include "db/sql/select.h"
#include "kernel/errorinfo.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

using sn::corelib::ErrorInfo;
using sn::corelib::db::sql::platform::Platform;
using sn::corelib::db::engine::Engine;
using sn::corelib::db::sql::AbstractSql;

class SN_CORELIB_EXPORT Sql
{
public:
   Sql(Engine &engine, const QString &table = QString());
   Sql(Engine &engine, const TableIdentifier &table);
   Engine& getEngine();
   bool hasTable();
   const QString& getTableName();
   const TableIdentifier& getTable();
   Sql& setTable(const TableIdentifier &table);
   Sql& setTable(const QString &table);
   QString buildSqlString(QSharedPointer<AbstractSql> sqlObject);
public:
   QSharedPointer<Delete> getDeleteSql(const QString &table = QString());
   QSharedPointer<Select> getSelectSql(const QString &table = QString())throw(ErrorInfo);
protected:
   Engine& m_engine;
   TableIdentifier m_table;
   Platform m_platfrom;
};

}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_SQL_H
