#ifndef SN_CORELIB_DB_SQL_DDL_ALTER_TABLE_H
#define SN_CORELIB_DB_SQL_DDL_ALTER_TABLE_H

#include <QList>
#include <QSharedPointer>
#include <QMap>

#include "global/global.h"
#include "db/sql/abstract_sql.h"
#include "db/sql/table_identifier.h"
#include "db/sql/ddl/column/abstract_column.h"
#include "db/sql/ddl/constraint/abstract_constraint.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace ddl{

using sn::corelib::db::sql::AbstractSql;
using sn::corelib::db::sql::TableIdentifier;
using sn::corelib::db::sql::ddl::column::AbstractColumn;
using sn::corelib::db::sql::ddl::constraint::AbstractConstraint;

class SN_CORELIB_EXPORT AlterTable : public AbstractSql
{
public:
   friend ProcessResultPointerType altertable_process_table(
         AbstractSql *self,const Engine &engine, 
         ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
         QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType altertable_process_add_columns(
         AbstractSql *self,const Engine &engine, 
         ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
         QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType altertable_process_change_columns(
         AbstractSql *self,const Engine &engine, 
         ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
         QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType altertable_process_drop_columns(
         AbstractSql *self,const Engine &engine, 
         ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
         QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType altertable_process_add_constraints(
         AbstractSql *self,const Engine &engine, 
         ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
         QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType altertable_process_drop_constraints(
         AbstractSql *self,const Engine &engine, 
         ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
         QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
public:
   const static QString ADD_COLUMNS;
   const static QString ADD_CONSTRAINTS;
   const static QString CHANGE_COLUMNS;
   const static QString DROP_COLUMNS;
   const static QString DROP_CONSTRAINTS;
   const static QString TABLE;
   
   struct RawState
   {
      TableIdentifier table;
      QList<QSharedPointer<AbstractColumn>> addColumns;
      QMap<QString, QSharedPointer<AbstractColumn>> changColumns;
      QList<QString> dropColumns;
      QList<QString> dropConstraints;
      QList<QSharedPointer<AbstractConstraint>> addConstraints;
   };
public:
   AlterTable(const TableIdentifier &table = TableIdentifier());
   AlterTable(const QString &table, const QString &schema = QString());
   
   AlterTable& setTable(const TableIdentifier &table = TableIdentifier());
   AlterTable& setTable(const QString &table, const QString &schema = QString());
   
   AlterTable& addColumn(const QSharedPointer<AbstractColumn> column);
   AlterTable& changeColumn(const QString &name, const QSharedPointer<AbstractColumn> column);
   AlterTable& dropColumn(const QString &name);
   AlterTable& dropConstraint(const QString &name);
   AlterTable& addConstraint(const QSharedPointer<AbstractConstraint> constraint);
   
   RawState getRawState()const;
protected:
   TableIdentifier m_table;
   QList<QSharedPointer<AbstractColumn>> m_addColumns;
   QMap<QString, QSharedPointer<AbstractColumn>> m_changColumns;
   QList<QString> m_dropColumns;
   QList<QString> m_dropConstraints;
   QList<QSharedPointer<AbstractConstraint>> m_addConstraints;
};

}//ddl
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_DDL_ALTER_TABLE_H
