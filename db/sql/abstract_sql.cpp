#include <QRegularExpression>
#include <QMetaType>

#include "abstract_sql.h"
#include "table_identifier.h"
#include "abstract_expression.h"
#include "expression.h"
#include "global/common_funcs.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

using sn::corelib::instanceof;

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

QString AbstractSql::processExpression(const QSharedPointer<AbstractExpression> expression, const engine::Engine &engine, 
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
   AbstractExpression::ExpressionDataType expressionData = expression->getExpressionData();
   if(!m_instanceParameterIndex.contains(namedParameterPrefix)){
      m_instanceParameterIndex.insert(namedParameterPrefix, 1);
   }
   QVariant &expressionParamIndex = m_instanceParameterIndex[namedParameterPrefix];
   int partCount = expressionData.size();
   for(int i = 0; i < partCount; i++){
      QVariant &part = expressionData[i];
      if(part.type() == QVariant::String && instanceof<Expression>(expression)){
         Expression* exprPointer = qobject_cast<Expression*>(expression.data());
         if(0 != exprPointer){
            sql += exprPointer->getExpression();
         }else{
            Q_ASSERT_X(0 != exprPointer, "AbstractSql::processExpression", 
                       "Expression* exprPointer = qobject_cast<Expression*>(expression.data()); fail");
         }
         continue;
      }
      if(part.type() == QVariant::String){
         sql += part.toString();
         continue;
      }
      // Process values and types (the middle and last position of the
      // expression data)
      AbstractExpression::ExpressionDataType subParts = part.toList();
      QList<QVariant> types;
      QList<QVariant> values = subParts[1].toList();
      QStringList strValues;
      if(subParts.size() > 2){
         types = subParts[2].toList();
      }
      int valueCount = values.size();
      int typeCount = types.size();
      for(int vIndex = 0; vIndex < valueCount; vIndex++){
         const QVariant &value = values.at(vIndex);
         if(vIndex == typeCount || types.at(vIndex).isNull()){
            continue;
         }
         QString type = types.at(vIndex).toString();
         if(type == AbstractExpression::TYPE_IDENTIFIER){
            
         }else if(type == AbstractExpression::TYPE_VALUE){
            
         }else if(type == AbstractExpression::TYPE_LITERAL){
            strValues[vIndex] = value.toString();
         }
      }
      // After looping the values, interpolate them into the sql string
      // (they might be placeholder names, or values)
      QString tpl = subParts[0].toString();
      switch (valueCount) {
      case 1:
      {
         tpl = tpl.arg(values.at(0).toString());
         break;
      }
      case 2:
      {
         tpl = tpl.arg(values.at(0).toString(), values.at(1).toString());
         break;
      }
      case 3:
      {
         tpl = tpl.arg(values.at(0).toString(), values.at(1).toString(), 
                       values.at(2).toString());
      }
      case 4:
      {
         tpl = tpl.arg(values.at(0).toString(), values.at(1).toString(), 
                       values.at(2).toString(), values.at(3).toString());
      }
      case 5:
      {
         tpl = tpl.arg(values.at(0).toString(), values.at(1).toString(), 
                       values.at(2).toString(), values.at(3).toString(),
                       values.at(4).toString());
      }
      case 6:
      {
         tpl = tpl.arg(values.at(0).toString(), values.at(1).toString(), 
                       values.at(2).toString(), values.at(3).toString(),
                       values.at(4).toString(), values.at(5).toString());
      }
      case 7:
      {
         tpl = tpl.arg(values.at(0).toString(), values.at(1).toString(), 
                       values.at(2).toString(), values.at(3).toString(),
                       values.at(4).toString(), values.at(5).toString(),
                       values.at(6).toString());
      }
      case 8:
      {
         tpl = tpl.arg(values.at(0).toString(), values.at(1).toString(), 
                       values.at(2).toString(), values.at(3).toString(),
                       values.at(4).toString(), values.at(5).toString(),
                       values.at(6).toString(), values.at(7).toString());
      }
      case 9:
      {
         tpl = tpl.arg(values.at(0).toString(), values.at(1).toString(), 
                       values.at(2).toString(), values.at(3).toString(),
                       values.at(4).toString(), values.at(5).toString(),
                       values.at(6).toString(), values.at(7).toString(),
                       values.at(8).toString());
      }
      }
      sql += tpl;
   }
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
