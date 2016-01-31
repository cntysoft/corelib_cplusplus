#ifndef SN_CORELIB_DB_SQL_DDL_CREATE_TABLE_H
#define SN_CORELIB_DB_SQL_DDL_CREATE_TABLE_H

#include <QList>
#include <QSharedPointer>

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

class SN_CORELIB_EXPORT CreateTable : public AbstractSql
{
public:
   friend ProcessResultPointerType createtable_process_table(AbstractSql *self,const Engine &engine, 
                                                             ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                             QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType createtable_process_columns(AbstractSql *self,const Engine &engine, 
                                                               ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                               QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType createtable_process_combinedby(AbstractSql *self,const Engine &engine, 
                                                                  ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                                  QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType createtable_process_constraints(AbstractSql *self,const Engine &engine, 
                                                                   ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                                   QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType createtable_process_statementend(AbstractSql *self,const Engine &engine, 
                                                                    ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                                    QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
public:
   const static QString COLUMNS;
   const static QString CONSTRAINTS;
   const static QString TABLE;
   
   struct RawState
   {
      TableIdentifier table;
      QList<QSharedPointer<AbstractColumn>> columns;
      QList<QSharedPointer<AbstractConstraint>> constraints;
   };
   
public:
   CreateTable(const TableIdentifier &table = TableIdentifier(), bool isTemporary = false);
   CreateTable(const QString &table, const QString &schema = QString(), bool isTemporary = false);
   CreateTable& setTemporary(bool temporary);
   bool isTemporary()const;
   CreateTable& setTable(const TableIdentifier &table = TableIdentifier());
   CreateTable& setTable(const QString &table, const QString &schema = QString());
   CreateTable& addColumn(QSharedPointer<AbstractColumn> column);
   CreateTable& addConstraint(QSharedPointer<AbstractConstraint> constraint);
   RawState getRawState()const;
public:
   virtual QString getDecoratorClassName()const;
protected:
   virtual void localizeVariables();
protected:
   TableIdentifier m_table;
   QList<QSharedPointer<AbstractColumn>> m_columns;
   QList<QSharedPointer<AbstractConstraint>> m_constraints;
   bool m_isTemporary = false;
};

}//ddl
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_DDL_CREATE_TABLE_H
