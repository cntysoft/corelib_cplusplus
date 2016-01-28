#include "global/common_funcs.h"

#include "insert.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

using sn::corelib::is_scalar;

const QString Insert::SPECIFICATION_INSERT = "insert";
const QString Insert::SPECIFICATION_SELECT = "select";
const QString Insert::VALUES_MERGE = "merge";
const QString Insert::VALUES_SET   = "set";

AbstractSql::ProcessResultPointerType insert_process_insert(AbstractSql *self,const Engine &engine, 
                                                         ParameterContainer *parameterContainer, QMap<QString, QString>&, 
                                                         QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Insert* insertSql = dynamic_cast<Insert*>(self);
   Q_ASSERT_X(insertSql != 0, "friend function select_process_insert", "self pointer cast failure");
   if(0 == insertSql){
      throw ErrorInfo(QString("friend function select_process_insert self pointer cast failure"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(!insertSql->m_select.isNull()){
      result->isNull = true;
      return result;
   }
   Q_ASSERT_X(!insertSql->m_columns.isEmpty(), "insert_process_insert", "values or select should be present");
   if(insertSql->m_columns.isEmpty()){
      result->isNull = true;
      return result;
   }
   QMap<QString, QVariant>::const_iterator it = insertSql->m_columns.cbegin();
   QMap<QString, QVariant>::const_iterator endMarker = insertSql->m_columns.cend();
   QStringList columns;
   QStringList values;
   while(it != endMarker){
      QString columnName(it.key());
      QVariant value = it.value();
      columns.append(engine.quoteFieldName(columnName));
      if(is_scalar(value) && nullptr != parameterContainer){
         values.append(engine.formatParameterName(columnName));
         parameterContainer->offsetSet("column", value);
      }else{
         
         values.append(insertSql->resolveColumnValue(value, engine, parameterContainer));
      }
      it++;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::String;
   QString sql = QString(insertSql->m_specifications.value(Insert::SPECIFICATION_INSERT).toString())
         .arg(insertSql->resolveTable(insertSql->m_table, engine, parameterContainer))
         .arg(columns.join(", "))
         .arg(values.join(", "));
   result->value = QVariant(sql);
   return result;
}

AbstractSql::ProcessResultPointerType insert_process_select(AbstractSql *self,const Engine &engine, 
                                                         ParameterContainer *parameterContainer, QMap<QString, QString>&, 
                                                         QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Insert* insertSql = dynamic_cast<Insert*>(self);
   Q_ASSERT_X(insertSql != 0, "friend function select_process_insert", "self pointer cast failure");
   if(0 == insertSql){
      throw ErrorInfo(QString("friend function select_process_insert self pointer cast failure"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(insertSql->m_select.isNull()){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::String;
   QString selectSql = insertSql->processSubSelect(insertSql->m_select, engine, parameterContainer);
   QStringList columns;
   QMap<QString, QVariant>::const_iterator it = insertSql->m_columns.cbegin();
   QMap<QString, QVariant>::const_iterator endMarker = insertSql->m_columns.cend();
   while(it != endMarker){
      columns.append(engine.quoteFieldName(it.key()));
      it++;
   }
   QString sql = QString(insertSql->m_specifications.value(Insert::SPECIFICATION_SELECT).toString())
         .arg(insertSql->resolveTable(insertSql->m_table, engine, parameterContainer))
         .arg(!columns.isEmpty() ? QString("(%1)").arg(columns.join(", ")) : "")
         .arg(selectSql);
   result->value = QVariant(sql);
   return result;
}

Insert::Insert(const QString &table, const QString &schema)
   : Insert(TableIdentifier(table, schema))
{
}

Insert::Insert(const TableIdentifier &table)
{
   if(!table.isNull()){
      into(table);
   }
   m_specifications.insert(Insert::SPECIFICATION_INSERT, QVariant(QString("INSERT INTO %1 (%2) VALUES (%3)")));
   m_specifications.insert(Insert::SPECIFICATION_SELECT, QVariant(QString("INSERT INTO %1 %2 %3")));
   m_specificationFnPtrs.insert(Insert::SPECIFICATION_INSERT, insert_process_insert);
   m_specificationFnPtrs.insert(Insert::SPECIFICATION_SELECT, insert_process_select);
   m_specKeys.append(Insert::SPECIFICATION_INSERT);
   m_specKeys.append(Insert::SPECIFICATION_SELECT);
}

Insert& Insert::into(const QString &table, const QString &schema)
{
   return into(TableIdentifier(table, schema));
}

Insert& Insert::into(const TableIdentifier &table)
{
   m_table = table;
   return *this;
}

Insert& Insert::addColumn(const QString &name, const QVariant &value)
{
   m_columns.insert(name, value);
   return *this;
}

Insert& Insert::removeColumn(const QString &name)
{
   if(m_columns.contains(name)){
      m_columns.remove(name);
   }
   return *this;
}

Insert& Insert::columns(const QMap<QString, QVariant> &columns)
{
   m_columns = columns;
   return *this;
}

QVariant Insert::getColumn(const QString &name)
{
   if(m_columns.contains(name)){
      return m_columns.value(name);
   }
   return QVariant();
}

bool Insert::hasColumn(const QString &name)
{
   return m_columns.contains(name);
}

Insert& Insert::values(const QMap<QString, QVariant> &values, const QString flag)throw(ErrorInfo)
{
   if(!m_select.isNull() && Insert::VALUES_MERGE == flag){
      throw ErrorInfo("An array of values cannot be provided with the merge flag when a "
                      "sn::corelib::db::sql::Select instance already exists as the value source");
   }
   if(Insert::VALUES_SET == flag){
      m_columns = values;
   }else if(Insert::VALUES_MERGE == flag){
      QMap<QString, QVariant>::const_iterator it = values.cbegin();
      QMap<QString, QVariant>::const_iterator endMarker = values.cend();
      while(it != endMarker){
         m_columns.insert(it.key(), it.value());
         it++;
      }
   }
   return *this;
}

Insert& Insert::values(const QSharedPointer<Select> &select, const QString flag)throw(ErrorInfo)
{
   if(Insert::VALUES_MERGE == flag){
      throw ErrorInfo("A sn::corelib::db::sql::Select instance cannot be provided with the merge flag");
   }
   m_select = select;
   return *this;
}

Insert::RawState Insert::getRawState()const
{
   RawState state;
   state.table = m_table;
   state.columns = m_columns.keys();
   state.values = m_columns.values();
   return state;
}

}//sql
}//db
}//corelib
}//sn