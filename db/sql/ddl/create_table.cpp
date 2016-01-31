#include <algorithm>

#include "create_table.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace ddl{

const QString CreateTable::COLUMNS = "columns";
const QString CreateTable::CONSTRAINTS = "constraints";
const QString CreateTable::TABLE = "table";

AbstractSql::ProcessResultPointerType createtable_process_table(AbstractSql *self, const Engine &engine, ParameterContainer*, QMap<QString, QString>&, QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   CreateTable* createSql = dynamic_cast<CreateTable*>(self);
   Q_ASSERT_X(createSql != 0, "friend function createtable_process_table", "self pointer cast fail");
   if(0 == createSql){
      throw ErrorInfo(QString("friend function createtable_process_table self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   result->value = QVariant(QList<QVariant>{
                               createSql->isTemporary() ? "TEMPORARY" : "",
                               engine.quoteIdentifierInFragment(createSql->m_table.toString())
                            });
   return result;
}

AbstractSql::ProcessResultPointerType createtable_process_columns(AbstractSql *self, const Engine &engine, ParameterContainer*, QMap<QString, QString>&, QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   CreateTable* createSql = dynamic_cast<CreateTable*>(self);
   Q_ASSERT_X(createSql != 0, "friend function createtable_process_columns", "self pointer cast fail");
   if(0 == createSql){
      throw ErrorInfo(QString("friend function createtable_process_columns self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   result->type = AbstractSql::ProcessResultType::Array;
   if(createSql->m_columns.isEmpty()){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   QList<QVariant> sqls;
   std::for_each(createSql->m_columns.cbegin(), createSql->m_columns.cend(), [&sqls, &createSql, &engine](const QSharedPointer<AbstractColumn> &column){
      sqls.append(createSql->processExpression(column, engine));
   });
   result->value = QVariant(QList<QVariant>{
                               sqls
                            });
   return result;
}

AbstractSql::ProcessResultPointerType createtable_process_combinedby(AbstractSql *self, const Engine&, ParameterContainer*, QMap<QString, QString>&, QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   CreateTable* createSql = dynamic_cast<CreateTable*>(self);
   Q_ASSERT_X(createSql != 0, "friend function createtable_process_combinedby", "self pointer cast fail");
   if(0 == createSql){
      throw ErrorInfo(QString("friend function createtable_process_combinedby self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(createSql->m_constraints.isEmpty() || createSql->m_columns.isEmpty()){
      result->isNull = true;
      return result;
   }
   result->type = AbstractSql::ProcessResultType::String;
   result->value = QVariant(QString(createSql->m_specifications.value("combinedBy").toString()));
   return result;
}

AbstractSql::ProcessResultPointerType createtable_process_constraints(AbstractSql *self, const Engine &engine, ParameterContainer*, QMap<QString, QString>&, QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   CreateTable* createSql = dynamic_cast<CreateTable*>(self);
   Q_ASSERT_X(createSql != 0, "friend function createtable_process_constraints", "self pointer cast fail");
   if(0 == createSql){
      throw ErrorInfo(QString("friend function createtable_process_constraints self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(createSql->m_constraints.isEmpty()){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   QList<QVariant> sqls;
   std::for_each(createSql->m_constraints.cbegin(), createSql->m_constraints.cend(), [&sqls, &createSql, &engine](const QSharedPointer<AbstractConstraint> &constraint){
      sqls.append(createSql->processExpression(constraint, engine));
   });
   result->value.setValue(QList<QVariant>{
                               sqls
                            });
   return result;
}

AbstractSql::ProcessResultPointerType createtable_process_statementend(AbstractSql *self, const Engine&, ParameterContainer*, QMap<QString, QString>&, QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   CreateTable* createSql = dynamic_cast<CreateTable*>(self);
   Q_ASSERT_X(createSql != 0, "friend function createtable_process_statementend", "self pointer cast fail");
   if(0 == createSql){
      throw ErrorInfo(QString("friend function createtable_process_statementend self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   result->type = AbstractSql::ProcessResultType::String;
   result->value = QVariant(QString("\n)"));
   return result;
}


CreateTable::CreateTable(const QString &table, const QString &schema, bool isTemporary)
   : CreateTable(TableIdentifier(table, schema), isTemporary)
{}

CreateTable::CreateTable(const TableIdentifier &table, bool isTemporary)
   : m_table(table),
     m_isTemporary(isTemporary)
{
   m_specifications.insert(CreateTable::TABLE, "CREATE %1TABLE %2 (");
   QVariant columnSpecs{
      QMap<QString, QVariant>{
         {
            "\n    %1",
            QList<QVariant>{
               QMap<QString, QVariant>{
                  {"1" , "%1"},
                  {"combinedby", ",\n    "}
               }
            }
         }
      }
   };
   m_specifications.insert(CreateTable::COLUMNS, columnSpecs);
   m_specifications.insert("combinedBy", ",");
   QVariant constraintSpecs{
      QMap<QString, QVariant>{
         {
            "\n    %1",
            QList<QVariant>{
               QMap<QString, QVariant>{
                  {"1" , "%1"},
                  {"combinedby", ",\n    "}
               }
            }
         }
      }
   };
   m_specifications.insert(CreateTable::CONSTRAINTS, constraintSpecs);
   m_specifications.insert("statementEnd", "%1");
   
   m_specificationFnPtrs.insert(CreateTable::TABLE, createtable_process_table);
   m_specificationFnPtrs.insert(CreateTable::COLUMNS, createtable_process_columns);
   m_specificationFnPtrs.insert("combinedBy", createtable_process_combinedby);
   m_specificationFnPtrs.insert(CreateTable::CONSTRAINTS, createtable_process_constraints);
   m_specificationFnPtrs.insert("statementEnd", createtable_process_statementend);
   
   m_specKeys.append(CreateTable::TABLE);
   m_specKeys.append(CreateTable::COLUMNS);
   m_specKeys.append("combinedBy");
   m_specKeys.append(CreateTable::CONSTRAINTS);
   m_specKeys.append("statementEnd");
   
}

CreateTable& CreateTable::setTemporary(bool temporary)
{
   m_isTemporary = temporary;
   return *this;
}

bool CreateTable::isTemporary()const
{
   return m_isTemporary;
}

CreateTable& CreateTable::setTable(const QString &table, const QString &schema)
{
   return setTable(TableIdentifier(table, schema));
}

CreateTable& CreateTable::setTable(const TableIdentifier &table)
{
   m_table = table;
   return *this;
}

CreateTable& CreateTable::addColumn(QSharedPointer<AbstractColumn> column)
{
   m_columns.append(column);
   return *this;
}

CreateTable& CreateTable::addConstraint(QSharedPointer<AbstractConstraint> constraint)
{
   m_constraints.append(constraint);
   return *this;
}

CreateTable::RawState CreateTable::getRawState()const
{
   RawState state;
   state.table = m_table;
   state.columns = m_columns;
   state.constraints = m_constraints;
   return state;
}

QString CreateTable::getDecoratorClassName()const
{
   return QString("sn::corelib::db::sql::platform::mysql::CreateTableDecorator");
}

void CreateTable::localizeVariables()
{
   if(!m_isNeedLocalizeVariables || m_subject.isNull()){
      return;
   }
   AbstractSql::localizeVariables();
   QSharedPointer<CreateTable> castedCreateTableSubject = m_subject.dynamicCast<CreateTable>();
   Q_ASSERT_X(!castedCreateTableSubject.isNull(), "CreateTable::localizeVariables", "downcast failure");
   m_table= castedCreateTableSubject->m_table;
   m_columns = castedCreateTableSubject->m_columns;
   m_constraints = castedCreateTableSubject->m_constraints;
   m_isTemporary = castedCreateTableSubject->m_isTemporary;
}

}//ddl
}//sql
}//db
}//corelib
}//sn