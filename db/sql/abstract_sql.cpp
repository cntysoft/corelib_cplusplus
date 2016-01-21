#include <QRegularExpression>

#include "abstract_sql.h"
#include "table_identifier.h"
#include "abstract_expression.h"

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
      ProcessResultPointerType resultItem = parameters[key];
      if(!specification.isNull() && !resultItem->isNull && resultItem->type == ProcessResultType::Array){
         sqls[key] = createSqlFromSpecificationAndParameters(specification, resultItem->getValue().toMap());
         continue;
      }
      if(!resultItem->isNull && ProcessResultType::String == resultItem->type){
         sqls.insert(key, resultItem->getValue().toString());
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

QString AbstractSql::processExpression(const AbstractExpression &expression, const engine::Engine &engine, 
                                       const engine::ParameterContainer &parameterContainer, QString namedParameterPrefix)
{
   if(namedParameterPrefix.isNull()){
      namedParameterPrefix = m_processInfo.value("paramPrefix").toString() + namedParameterPrefix;
   }
   static int runtimeExpressionPrefix = 0;
   if(parameterContainer.count() > 0 && namedParameterPrefix.isEmpty()){
      namedParameterPrefix = QString("expr%1Param").arg(++runtimeExpressionPrefix);
   }else{
      namedParameterPrefix.replace(QRegularExpression("\\s"), "__");
   }
   QString sql;
   AbstractExpression::ExpressionDataType expressionData = expression.getExpressionData();
   if(!m_instanceParameterIndex.contains(namedParameterPrefix)){
      m_instanceParameterIndex.insert(namedParameterPrefix, 1);
   }
   QVariant &expressionParamIndex = m_instanceParameterIndex[namedParameterPrefix];
   return sql;
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
