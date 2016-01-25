#include <QRegularExpression>
#include <QMetaType>
#include <algorithm>

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
using sn::corelib::format_str;

AbstractSql& AbstractSql::setSpecificationFn(const QString &name, SpecificationFuncPtr fn)
{
   if(!m_specificationFnPtrs.contains(name)){
      m_specificationFnPtrs.insert(name, fn);
   }
   return *this;
}

QString AbstractSql::buildSqlString(const Engine &engine, ParameterContainer *parameterContainer)
{
   QMap<QString, QString> sqls;
   QMap<QString, ProcessResultPointerType> parameters;
   QStringList::const_iterator keyIterator = m_specKeys.cbegin();
   QStringList::const_iterator keyItEndMarker = m_specKeys.cend();
   while(keyIterator != keyItEndMarker){
      QString key(*keyIterator);
      SpecificationFuncPtr fn = nullptr;
      QVariant specification = m_specifications.value(key);
      if(m_specificationFnPtrs.contains(key)){
         fn = m_specificationFnPtrs.value(key);
      }
      Q_ASSERT_X(fn != nullptr, "AbstractSql::buildSqlString", "specification function can not be nullptr");
      parameters[key] = fn(this, engine, parameterContainer, sqls, parameters);
      ProcessResultPointerType resultItem = parameters[key];
      if(!specification.isNull() && !resultItem->isNull && resultItem->type == ProcessResultType::Array){
         sqls[key] = createSqlFromSpecificationAndParameters(specification, resultItem->getValue().toList());
         keyIterator++;
         continue;
      }
      if(!resultItem->isNull && ProcessResultType::String == resultItem->type){
         sqls.insert(key, resultItem->getValue().toString());
      }
      keyIterator++;
   }
   QStringList sqlList;
   std::for_each(m_specKeys.cbegin(), m_specKeys.cend(), [&sqlList, &sqls](const QString &key){
      if(sqls.contains(key)){
         sqlList.append(sqls.value(key));
      }
   });
   
   return sqlList.join(' ');
}

QString AbstractSql::createSqlFromSpecificationAndParameters(const QVariant &specification, const QList<QVariant> &parameters)throw(ErrorInfo)
{
   if(specification.type() == QVariant::String){
      QStringList strParams;
      std::for_each(parameters.cbegin(), parameters.cend(), [&strParams](const QVariant& param){
         strParams.append(param.toString());
      });
      //这里不需要转义吗？
      return format_str(specification.toString(), strParams);
   }
   int parameterCount = parameters.size();
   QMap<QString, QVariant> specifications = specification.toMap();
   QString specificationString;
   QVariant vparamSpecs;
   QMap<QString, QVariant>::const_iterator it = specifications.cbegin();
   QMap<QString, QVariant>::const_iterator endMark = specifications.cend();
   while(it != endMark){
      const QVariant &item = it.value();
      int paramSpecCount = 0;
      if(item.type() == QVariant::List){
         paramSpecCount = item.toList().size();
      }else if(item.type() == QVariant::Map){
         paramSpecCount = item.toMap().size();
      }
      if(paramSpecCount == parameterCount){
         vparamSpecs = item;
         specificationString = it.key();
         break;
      }
      it++;
   }
   if(specificationString.isNull()){
      throw ErrorInfo("A number of parameters was found that is not supported by this specification");
   }
   QStringList topParameters;
   QList<QVariant> paramSpecs = vparamSpecs.toList();
   for(int i = 0; i < parameterCount; i++){
      const QVariant & vparamsForPosition = parameters.at(i);
      if(vparamsForPosition.type() == QVariant::List){
         QList<QVariant> paramsForPosition = parameters.at(i).toList();
         int paramForPositionCount = paramsForPosition.size();
         if(i < paramSpecs.size()){
            QVariant vcurrentSpec = paramSpecs.at(i);
            if(!vcurrentSpec.isNull() && vcurrentSpec.type() == QVariant::Map){
               QMap<QString, QVariant> currentSpec = paramSpecs.at(i).toMap();
               if(currentSpec.contains("combinedby")){
                  QStringList multiParamValues;
                  for(int j = 0; j < paramForPositionCount; j++){
                     const QVariant &vparamsForPositionItem = paramsForPosition.at(j);
                     QStringList paramsForPositionItem = vparamsForPositionItem.toStringList();
                     int ppCount = paramsForPositionItem.size();
                     if(!paramSpecs[i].toMap().contains(QString("%1").arg(ppCount))){
                        throw ErrorInfo(QString("A number of parameters (%1) was found that is not supported by this specification").arg(ppCount));
                     }
                     multiParamValues << format_str(paramSpecs[i].toMap().value(QString("%1").arg(ppCount)).toString(), paramsForPositionItem);
                  }
                  topParameters << multiParamValues.join(paramSpecs[i].toMap().value("combinedby").toString());
               }
            }else if(!vcurrentSpec.isNull()){
               int ppCount = paramForPositionCount;
               if(!paramSpecs[i].toMap().contains(QString("%1").arg(ppCount))){
                  throw ErrorInfo(QString("A number of parameters (%1) was found that is not supported by this specification").arg(ppCount));
               }
               QStringList formatArgs;
               std::for_each(paramsForPosition.cbegin(), paramsForPosition.cend(), 
                             [&formatArgs](const QVariant &item){
                  formatArgs.append(item.toString());
               });
               topParameters << format_str(paramSpecs[i].toMap().value(QString("%1").arg(ppCount)).toString(), formatArgs);
            }
         }
      }else if(vparamsForPosition.type() == QVariant::String){
         QString paramsForPosition = parameters.at(i).toString();
         if(i < paramSpecs.size()){
            QVariant vcurrentSpec = paramSpecs.at(i);
            if(!vcurrentSpec.isNull() && vcurrentSpec.type() == QVariant::Map){
               QMap<QString, QVariant> currentSpec = paramSpecs.at(i).toMap();
               if(currentSpec.contains("combinedby")){
                  continue;
               }
            }else if(!vcurrentSpec.isNull()){
               continue;
            }else{
               topParameters << paramsForPosition;
            }
         }
      }
   }
   return format_str(specificationString, topParameters);
}

QString AbstractSql::resolveColumnValue(const QVariant &column, const Engine &engine, 
                                        ParameterContainer *parameterContainer, 
                                        QString namedParameterPrefix)
{
   namedParameterPrefix = (namedParameterPrefix.isNull() || namedParameterPrefix.isEmpty()) ? 
            namedParameterPrefix : m_processInfo.value("paramPrefix").toString() + namedParameterPrefix;
   bool isIdentifier = false;
   QString fromTable("");
   QVariant vcolumn;
   if(column.type() == QVariant::Map){
      QMap<QString, QVariant> columnMetaInfo = column.toMap();
      if(columnMetaInfo.contains("isIdentifier")){
         isIdentifier = columnMetaInfo.value("isIdentifier").toBool();
      }
      if(columnMetaInfo.contains("fromTable")){
         QString tempFromTable(columnMetaInfo.value("fromTable").toString());
         if(!tempFromTable.isNull() && !tempFromTable.isEmpty()){
            fromTable = tempFromTable;
         }
      }
      vcolumn = columnMetaInfo.value("column");
   }else{
      vcolumn = column;
   }
   if(vcolumn.isNull()){
      return "NULL";
   }
   //到这里应该是字符串了
   return isIdentifier ? fromTable+engine.quoteIdentifierInFragment(vcolumn.toString()) : engine.quoteValue(vcolumn.toString());
}

QString AbstractSql::getSqlString(const Engine &engine)
{
   return buildSqlString(engine);
}

QString AbstractSql::processExpression(const QSharedPointer<AbstractExpression> expression, const engine::Engine &engine, 
                                       ParameterContainer *parameterContainer, QString namedParameterPrefix)
{
   if(namedParameterPrefix.isNull()){
      namedParameterPrefix = m_processInfo.value("paramPrefix").toString() + namedParameterPrefix;
   }
   static int runtimeExpressionPrefix = 0;
   if(parameterContainer != nullptr && !parameterContainer->isEmpty() && namedParameterPrefix.isEmpty()){
      namedParameterPrefix = QString("expr%1Param").arg(++runtimeExpressionPrefix);
   }else{
      namedParameterPrefix.replace(QRegularExpression("\\s"), "__");
   }
   QString sql;
   AbstractExpression::ExpressionDataType expressionData = expression->getExpressionData();
   if(!m_instanceParameterIndex.contains(namedParameterPrefix)){
      m_instanceParameterIndex.insert(namedParameterPrefix, 1);
   }
   int &expressionParamIndex = m_instanceParameterIndex[namedParameterPrefix];
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
            strValues.insert(vIndex, engine.quoteIdentifierInFragment(value.toString()));
         }else if(type == AbstractExpression::TYPE_VALUE){
            // if prepareType is
            // passed back to the statement in a way it can be used as a placeholder value
            if(nullptr != parameterContainer && !parameterContainer->isEmpty()){
               QString name(namedParameterPrefix+QString("%1").arg(expressionParamIndex++));
               parameterContainer->offsetSet(name, value);
               strValues.insert(vIndex, "?");
               continue;
            }
            strValues.insert(vIndex, engine.quoteValue(value));
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
         tpl = tpl.arg(strValues.at(0));
         break;
      }
      case 2:
      {
         tpl = tpl.arg(strValues.at(0), strValues.at(1));
         break;
      }
      case 3:
      {
         tpl = tpl.arg(strValues.at(0), strValues.at(1), 
                       strValues.at(2));
         break;
      }
      case 4:
      {
         tpl = tpl.arg(strValues.at(0), strValues.at(1), 
                       strValues.at(2), strValues.at(3));
         break;
      }
      case 5:
      {
         tpl = tpl.arg(strValues.at(0), strValues.at(1), 
                       strValues.at(2), strValues.at(3),
                       strValues.at(4));
         break;
      }
      case 6:
      {
         tpl = tpl.arg(strValues.at(0), strValues.at(1), 
                       strValues.at(2), strValues.at(3),
                       strValues.at(4), strValues.at(5));
         break;
      }
      case 7:
      {
         tpl = tpl.arg(strValues.at(0), strValues.at(1), 
                       strValues.at(2), strValues.at(3),
                       strValues.at(4), strValues.at(5),
                       strValues.at(6));
         break;
      }
      case 8:
      {
         tpl = tpl.arg(strValues.at(0), strValues.at(1), 
                       strValues.at(2), strValues.at(3),
                       strValues.at(4), strValues.at(5),
                       strValues.at(6), strValues.at(7));
         break;
      }
      case 9:
      {
         tpl = tpl.arg(strValues.at(0), strValues.at(1), 
                       strValues.at(2), strValues.at(3),
                       strValues.at(4), strValues.at(5),
                       strValues.at(6), strValues.at(7),
                       strValues.at(8));
         break;
      }
      }
      sql += tpl;
   }
   return sql;
}

QString AbstractSql::resolveTable(const TableIdentifier &table, const Engine &engine, ParameterContainer *)
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
