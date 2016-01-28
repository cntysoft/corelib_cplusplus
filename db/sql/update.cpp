#include "update.h"
#include "global/common_funcs.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

const QString Update::SPECIFICATION_UPDATE = "update";
const QString Update::SPECIFICATION_WHERE = "where";
const QString Update::VALUES_MERGE = "merge";
const QString Update::VALUES_SET = "set";

using sn::corelib::is_scalar;

AbstractSql::ProcessResultPointerType update_process_update(AbstractSql *self,const Engine &engine, 
                                                            ParameterContainer *parameterContainer, QMap<QString, QString>&, 
                                                            QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Update* updateSql = dynamic_cast<Update*>(self);
   Q_ASSERT_X(updateSql != 0, "friend function update_process_update", "self pointer cast failure");
   if(0 == updateSql){
      throw ErrorInfo(QString("friend function update_process_update self pointer cast failure"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(updateSql->m_set.isEmpty()){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::String;
   QStringList setSql;
   QMap<QString, QVariant>::const_iterator it = updateSql->m_set.cbegin();
   QMap<QString, QVariant>::const_iterator endMarker = updateSql->m_set.cend();
   while(it != endMarker){
      QString columnName(it.key());
      QVariant value(it.value());
      QString prefix(engine.quoteFieldName(columnName)+" = ");
      if(is_scalar(value) && nullptr != parameterContainer){
         setSql.append(prefix+engine.formatParameterName(columnName));
         parameterContainer->offsetSet(columnName, value);
      }else{
         setSql.append(prefix+updateSql->resolveColumnValue(value, engine, parameterContainer));
      }
      it++;
   }
   QString sql = QString(updateSql->m_specifications.value(Update::SPECIFICATION_UPDATE).toString())
         .arg(updateSql->resolveTable(updateSql->m_table, engine, parameterContainer))
         .arg(setSql.join(", "));
   result->value = QVariant(sql);
   return result;
}

AbstractSql::ProcessResultPointerType update_process_where(AbstractSql *self,const Engine &engine, 
                                                           ParameterContainer *parameterContainer, QMap<QString, QString>&, 
                                                           QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Update* updateSql = dynamic_cast<Update*>(self);
   Q_ASSERT_X(updateSql != 0, "friend function update_process_update", "self pointer cast failure");
   if(0 == updateSql){
      throw ErrorInfo(QString("friend function update_process_update self pointer cast failure"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(updateSql->m_where->count() == 0){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::String;
   QString sql = QString(updateSql->m_specifications.value(Update::SPECIFICATION_WHERE).toString())
         .arg(updateSql->processExpression(updateSql->m_where, engine, parameterContainer, "where"));
   result->value = QVariant(sql);
   return result;
}

Update::Update(const QString &table, const QString &schema)
   : Update(TableIdentifier(table, schema))
{
}

Update::Update(const TableIdentifier &tableInfo)
   : m_where(new Where)
{
   if(!tableInfo.isNull()){
      table(tableInfo);
   }
   m_specifications.insert(Update::SPECIFICATION_UPDATE, QVariant(QString("UPDATE %1 SET %2")));
   m_specifications.insert(Update::SPECIFICATION_WHERE, QVariant(QString("WHERE %1")));
   m_specificationFnPtrs.insert(Update::SPECIFICATION_UPDATE, update_process_update);
   m_specificationFnPtrs.insert(Update::SPECIFICATION_WHERE, update_process_where);
   m_specKeys.append(Update::SPECIFICATION_UPDATE);
   m_specKeys.append(Update::SPECIFICATION_WHERE);
}

Update& Update::table(const QString &tableName, const QString &schema)
{
   return table(TableIdentifier(tableName, schema));
}

Update& Update::table(const TableIdentifier &tableInfo)
{
   m_table = tableInfo;
   return *this;
}

Update& Update::set(const QMap<QString, QVariant> &values, const QString &flag)
{
   if(values.isEmpty()){
      return *this;
   }
   if(Update::VALUES_SET == flag){
      m_set.clear();
   }
   QMap<QString, QVariant>::const_iterator it = values.cbegin();
   QMap<QString, QVariant>::const_iterator endMarker = values.cend();
   while(it != endMarker){
      m_set.insert(it.key(), it.value());
      it++;
   }
   return *this;
}


Update& Update::where(const QSharedPointer<Where> &where)
{
   m_where = where;
   return *this;
}

Update& Update::where(const QString &where, const QString &combination)
{
   m_where->addPredicate(where, combination);
   return *this;
}

Update::RawState Update::getRawState() const
{
   RawState state;
   state.emptyWhereProtection = m_emptyWhereProtection;
   state.set = m_set;
   state.table = m_table;
   state.where = m_where;
   return state;
}

}//sql
}//db
}//corelib
}//sn