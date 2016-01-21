#include "abstract_sql.h"
#include "table_identifier.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

AbstractSql& AbstractSql::setSpecificationFn(const QString &name, SpecificationFuncPtr fn)
{
   if(!m_specificationFnPtrs.contains(name)){
      m_specificationFnPtrs.insert(name, fn);
   }
   return *this;
}

QString AbstractSql::buildSqlString(const Engine &engine, const ParameterContainer &parameterContainer)
{
   QMap<QString, QString> sqls;
   QMap<QString, ProcessResultPointerType> parameters;
   QMap<QString, QVariant>::const_iterator cit = m_specifications.cbegin();
   while(cit != m_specifications.cend()){
      SpecificationFuncPtr fn = nullptr;
      QString key(cit.key());
      QVariant specification = cit.value();
      if(m_specificationFnPtrs.contains(key)){
         fn = m_specificationFnPtrs.value(key);
      }
      Q_ASSERT_X(fn != nullptr, "AbstractSql::buildSqlString", "specification function can not be nullptr");
      parameters[key] = fn(this, engine, parameterContainer, sqls, parameters);
      if(!specification.isNull() && parameters.value(key)->type == ProcessResultType::Array){
         sqls[key] = createSqlFromSpecificationAndParameters(specification, parameters[key]->getValue().toMap());
         continue;
      }
      if(ProcessResultType::String == parameters.value(key)->type){
         sqls.insert(key, parameters.value(key)->getValue().toString());
      }
      cit++;
   }
   return sqls.values().join(' ');
}

QString AbstractSql::createSqlFromSpecificationAndParameters(const QVariant &specification, const QMap<QString, QVariant> &parameters)
{
   return QString();
}

QString AbstractSql::getSqlString(const Engine &engine)
{
   return buildSqlString(engine);
}

QString AbstractSql::resolveTable(const TableIdentifier &table, const Engine &engine, const ParameterContainer&)
{
   //暂时不支持子查询
   QString schema(table.getSchema());
   QString tableName(table.getTable());
   if(!tableName.isNull()){
      tableName = engine.quoteIdentifier(tableName, Engine::IdentifierType::TableName);
   }
   if(!schema.isNull() && !tableName.isNull()){
      tableName = engine.quoteTableName(schema)+engine.getIdentifierSeparator()+engine.quoteTableName(tableName);
   }
   return tableName;
}

}//sql
}//db
}//corelib
}//sn
