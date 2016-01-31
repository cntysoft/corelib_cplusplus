#include <algorithm>

#include "alter_table.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace ddl{

const QString AlterTable::ADD_COLUMNS      = "addColumns";
const QString AlterTable::ADD_CONSTRAINTS  = "addConstraints";
const QString AlterTable::CHANGE_COLUMNS   = "changeColumns";
const QString AlterTable::DROP_COLUMNS     = "dropColumns";
const QString AlterTable::DROP_CONSTRAINTS = "dropConstraints";
const QString AlterTable::TABLE            = "table";

AbstractSql::ProcessResultPointerType altertable_process_table(
      AbstractSql *self,const Engine &engine, 
      ParameterContainer*, QMap<QString, QString>&, 
      QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   AlterTable* alterTableSql = dynamic_cast<AlterTable*>(self);
   Q_ASSERT_X(alterTableSql != 0, "friend function altertable_process_table", "self pointer cast fail");
   if(0 == alterTableSql){
      throw ErrorInfo(QString("friend function altertable_process_table self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   result->value.setValue(QList<QVariant>{
                               engine.quoteIdentifierInFragment(alterTableSql->m_table.toString())
                            });
   return result;
}

AbstractSql::ProcessResultPointerType altertable_process_add_columns(
      AbstractSql *self,const Engine &engine, 
      ParameterContainer*, QMap<QString, QString>&, 
      QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   AlterTable* alterTableSql = dynamic_cast<AlterTable*>(self);
   Q_ASSERT_X(alterTableSql != 0, "friend function altertable_process_add_columns", "self pointer cast fail");
   if(0 == alterTableSql){
      throw ErrorInfo(QString("friend function altertable_process_add_columns self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(alterTableSql->m_addColumns.isEmpty()){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   QList<QVariant> sqls;
   std::for_each(alterTableSql->m_addColumns.cbegin(), alterTableSql->m_addColumns.cend(), [&sqls, &alterTableSql, &engine](const QSharedPointer<AbstractColumn> &column){
      sqls.append(alterTableSql->processExpression(column, engine));
   });
   result->value.setValue(QList<QVariant>{
                             sqls 
                          });
   return result;
}

AbstractSql::ProcessResultPointerType altertable_process_change_columns(
      AbstractSql *self,const Engine &engine, 
      ParameterContainer*, QMap<QString, QString>&, 
      QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   AlterTable* alterTableSql = dynamic_cast<AlterTable*>(self);
   Q_ASSERT_X(alterTableSql != 0, "friend function altertable_process_change_columns", "self pointer cast fail");
   if(0 == alterTableSql){
      throw ErrorInfo(QString("friend function altertable_process_change_columns self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(alterTableSql->m_changColumns.isEmpty()){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   QList<QVariant> sqls;
   QMap<QString, QSharedPointer<AbstractColumn>>::const_iterator it = alterTableSql->m_changColumns.cbegin();
   QMap<QString, QSharedPointer<AbstractColumn>>::const_iterator endMarker = alterTableSql->m_changColumns.cend();
   while(it != endMarker){
      sqls.append(QList<QVariant>{
                     engine.quoteFieldName(it.key()),
                     alterTableSql->processExpression(it.value(), engine)
                  });
      it++;
   }
   result->value.setValue(QList<QVariant>{
                             sqls 
                          });
   return result;
}

AbstractSql::ProcessResultPointerType altertable_process_drop_columns(
      AbstractSql *self,const Engine &engine, 
      ParameterContainer*, QMap<QString, QString>&, 
      QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   AlterTable* alterTableSql = dynamic_cast<AlterTable*>(self);
   Q_ASSERT_X(alterTableSql != 0, "friend function altertable_process_drop_columns", "self pointer cast fail");
   if(0 == alterTableSql){
      throw ErrorInfo(QString("friend function altertable_process_drop_columns self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(alterTableSql->m_dropColumns.isEmpty()){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   QList<QVariant> sqls;
   std::for_each(alterTableSql->m_dropColumns.cbegin(), alterTableSql->m_dropColumns.cend(), [&sqls, &engine](const QString &column){
      sqls.append(engine.quoteFieldName(column));
   });
   result->value.setValue(QList<QVariant>{
                             sqls 
                          });
   return result;
}

AbstractSql::ProcessResultPointerType altertable_process_add_constraints(
      AbstractSql *self,const Engine &engine, 
      ParameterContainer*, QMap<QString, QString>&, 
      QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   AlterTable* alterTableSql = dynamic_cast<AlterTable*>(self);
   Q_ASSERT_X(alterTableSql != 0, "friend function altertable_process_add_constraints", "self pointer cast fail");
   if(0 == alterTableSql){
      throw ErrorInfo(QString("friend function altertable_process_add_constraints self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(alterTableSql->m_addConstraints.isEmpty()){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   QList<QVariant> sqls;
   std::for_each(alterTableSql->m_addConstraints.cbegin(), alterTableSql->m_addConstraints.cend(), [&sqls, &alterTableSql, &engine](const QSharedPointer<AbstractConstraint> &constraint){
      sqls.append(alterTableSql->processExpression(constraint, engine));
   });
   result->value.setValue(QList<QVariant>{
                             sqls 
                          });
   return result;
}

AbstractSql::ProcessResultPointerType altertable_process_drop_constraints(
      AbstractSql *self,const Engine &engine, 
      ParameterContainer*, QMap<QString, QString>&, 
      QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   AlterTable* alterTableSql = dynamic_cast<AlterTable*>(self);
   Q_ASSERT_X(alterTableSql != 0, "friend function altertable_process_drop_constraints", "self pointer cast fail");
   if(0 == alterTableSql){
      throw ErrorInfo(QString("friend function altertable_process_drop_constraints self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(alterTableSql->m_dropConstraints.isEmpty()){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   QList<QVariant> sqls;
   std::for_each(alterTableSql->m_dropConstraints.cbegin(), alterTableSql->m_dropConstraints.cend(), [&sqls, &engine](const QString &constraint){
      sqls.append(engine.quoteFieldName(constraint));
   });
   result->value.setValue(QList<QVariant>{
                             sqls 
                          });
   return result;
}

AlterTable::AlterTable(const QString &table, const QString &schema)
   : AlterTable(TableIdentifier(table, schema))
{
}

AlterTable::AlterTable(const TableIdentifier &table)
   : m_table(table)
{
   m_specifications.insert(AlterTable::TABLE, "ALTER TABLE %s\n");
   QMap<QString, QVariant> addColumnsSpec{
      {"%1", QList<QVariant>{
            QMap<QString, QVariant>{
               {"1", "ADD COLUMN %s,\n"}, 
               {"combinedby" , ""}
            }
         }
      }
   };
   m_specifications.insert(AlterTable::ADD_COLUMNS, addColumnsSpec);
   
   QMap<QString, QVariant> changeColumnsSpec{
      {"%1", QList<QVariant>{
            QMap<QString, QVariant>{
               {"2", "CHANGE COLUMN %s %s,\n"}, 
               {"combinedby" , ""}
            }
         }
      }
   };
   m_specifications.insert(AlterTable::CHANGE_COLUMNS, changeColumnsSpec);
   
   QMap<QString, QVariant> dropColumnsSpec{
      {"%1", QList<QVariant>{
            QMap<QString, QVariant>{
               {"1", "DROP COLUMN %s,\n"}, 
               {"combinedby" , ""}
            }
         }
      }
   };
   m_specifications.insert(AlterTable::DROP_COLUMNS, dropColumnsSpec);
   
   QMap<QString, QVariant> addConstraintsSpec{
      {"%1", QList<QVariant>{
            QMap<QString, QVariant>{
               {"1", "ADD %s,\n"}, 
               {"combinedby" , ""}
            }
         }
      }
   };
   m_specifications.insert(AlterTable::ADD_CONSTRAINTS, addConstraintsSpec);
   
   QMap<QString, QVariant> dropConstraintsSpec{
      {"%1", QList<QVariant>{
            QMap<QString, QVariant>{
               {"1", "DROP CONSTRAINT %s,\n"}, 
               {"combinedby" , ""}
            }
         }
      }
   };
   m_specifications.insert(AlterTable::DROP_CONSTRAINTS, dropConstraintsSpec);
   
   m_specificationFnPtrs.insert(AlterTable::TABLE, altertable_process_table);
   m_specificationFnPtrs.insert(AlterTable::ADD_COLUMNS, altertable_process_add_columns);
   m_specificationFnPtrs.insert(AlterTable::CHANGE_COLUMNS, altertable_process_change_columns);
   m_specificationFnPtrs.insert(AlterTable::DROP_COLUMNS, altertable_process_drop_columns);
   m_specificationFnPtrs.insert(AlterTable::ADD_CONSTRAINTS, altertable_process_add_constraints);
   m_specificationFnPtrs.insert(AlterTable::DROP_CONSTRAINTS, altertable_process_drop_constraints);
   
   m_specKeys.append(AlterTable::TABLE);
   m_specKeys.append(AlterTable::ADD_COLUMNS);
   m_specKeys.append(AlterTable::CHANGE_COLUMNS);
   m_specKeys.append(AlterTable::DROP_COLUMNS);
   m_specKeys.append(AlterTable::ADD_CONSTRAINTS);
   m_specKeys.append(AlterTable::DROP_CONSTRAINTS);
}

AlterTable& AlterTable::setTable(const QString &table, const QString &schema)
{
   return setTable(TableIdentifier(table, schema));
}

AlterTable& AlterTable::setTable(const TableIdentifier &table)
{
   m_table = table;
   return *this;
}

AlterTable& AlterTable::addColumn(const QSharedPointer<AbstractColumn> column)
{
   m_addColumns.append(column);
   return *this;
}

AlterTable& AlterTable::changeColumn(const QString &name, const QSharedPointer<column::AbstractColumn> column)
{
   m_changColumns.insert(name, column);
   return *this;
}

AlterTable& AlterTable::dropColumn(const QString &name)
{
   m_dropColumns.append(name);
   return *this;
}

AlterTable& AlterTable::dropConstraint(const QString &name)
{
   m_dropConstraints.append(name);
   return *this;
}

AlterTable& AlterTable::addConstraint(const QSharedPointer<constraint::AbstractConstraint> constraint)
{
   m_addConstraints.append(constraint);
   return *this;
}

AlterTable::RawState AlterTable::getRawState()const
{
   RawState state;
   state.table = m_table;
   state.addColumns = m_addColumns;
   state.addConstraints = m_addConstraints;
   state.changColumns = m_changColumns;
   state.dropColumns = m_dropColumns;
   state.dropConstraints = m_dropConstraints;
   return state;
}

}//ddl
}//sql
}//db
}//corelib
}//sn