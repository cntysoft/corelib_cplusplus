#include <QRegularExpression>
#include <QMetaType>
#include <algorithm>

#include "abstract_sql.h"
#include "table_identifier.h"
#include "abstract_expression.h"
#include "expression.h"
#include "global/common_funcs.h"
#include "db/sql/select.h"
#include "db/sql/platform/abstract_platform.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

using sn::corelib::instanceof;
using sn::corelib::format_str;
using sn::corelib::db::sql::platform::PlatformDecoratorInterface;

AbstractSql& AbstractSql::setSpecificationFn(const QString &name, SpecificationFuncPtr fn)
{
   if(!m_specificationFnPtrs.contains(name)){
      m_specificationFnPtrs.insert(name, fn);
   }
   return *this;
}

QString AbstractSql::buildSqlString(const Engine &engine, ParameterContainer *parameterContainer)
{
   localizeVariables();
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
   
   return sqlList.join(' ').replace(QRegularExpression("[\\n|,]*$"), "");
}

void AbstractSql::localizeVariables()
{
   if(!m_isNeedLocalizeVariables || m_subject.isNull()){
      return;
   }
   m_specKeys = m_subject->m_specKeys;
   m_specifications = m_subject->m_specifications;
   m_processInfo = m_subject->m_processInfo;
   m_instanceParameterIndex = m_subject->m_instanceParameterIndex;
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
   if(vcolumn.userType() == qMetaTypeId<QSharedPointer<AbstractExpression>>()){
      return processExpression(vcolumn.value<QSharedPointer<AbstractExpression>>(), engine, parameterContainer, namedParameterPrefix);
   }
   
   if(vcolumn.userType() == qMetaTypeId<QSharedPointer<Select>>()){
      return "(" + processSubSelect(vcolumn.value<QSharedPointer<Select>>(), engine, parameterContainer)+ ")";
   }
   
   if(vcolumn.isNull()){
      return "NULL";
   }
   //到这里应该是字符串了
   return isIdentifier ? fromTable+engine.quoteIdentifierInFragment(vcolumn.toString()) : engine.quoteValue(vcolumn);
}


QString AbstractSql::processSubSelect(QSharedPointer<Select> subSelect, const Engine &engine, 
                                      ParameterContainer *parameterContainer)throw(ErrorInfo)
{
   QSharedPointer<Select> decorator;
   if(instanceof<PlatformDecoratorInterface>(*this)){
      Select *childPtr = dynamic_cast<Select*>(this);
      Q_ASSERT_X(childPtr != nullptr, "AbstractSql::processSubSelect", "this pointer cast failure");
      if(nullptr == childPtr){
         throw ErrorInfo("Select *childPtr = dynamic_cast<Select*>(this) failure");
      }
      decorator.reset(new Select(*childPtr));
      decorator->setSubject(subSelect);
   }else{
      decorator = subSelect;
   }
   if(nullptr != parameterContainer && !parameterContainer->isEmpty()){
      QSharedPointer<Select> processInfoContext;
      // Track subselect prefix and count for parameters
      if(instanceof<PlatformDecoratorInterface>(*decorator.data())){
         processInfoContext = subSelect;
      }else{
         processInfoContext = decorator;
      }
      int subselectCount = m_processInfo.value("subselectCount").toInt();
      subselectCount++;
      m_processInfo["subselectCount"].setValue(subselectCount);
      processInfoContext->m_processInfo["subselectCount"].setValue(subselectCount);
      processInfoContext->m_processInfo["paramPrefix"].setValue(QString("subselect") + QString("%1").arg(subselectCount));
      QString sql = decorator->buildSqlString(engine, parameterContainer);
      m_processInfo["subselectCount"].setValue(decorator->m_processInfo["subselectCount"].toInt());
      return sql;
   }
   return decorator->buildSqlString(engine, parameterContainer);
}

QString AbstractSql::getSqlString(const Engine &engine)
{
   return buildSqlString(engine);
}

AbstractSql& AbstractSql::setSubject(QSharedPointer<AbstractSql> subject)
{
   m_subject = subject;
   return *this;
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
         Expression* exprPointer = dynamic_cast<Expression*>(expression.data());
         if(0 != exprPointer){
            sql += exprPointer->getExpression();
         }else{
            Q_ASSERT_X(0 != exprPointer, "AbstractSql::processExpression", 
                       "Expression* exprPointer = dynamic_cast<Expression*>(expression.data()); fail");
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
         if(value.userType() == qMetaTypeId<QSharedPointer<Select>>()){
            // process sub-select
            strValues.insert(vIndex,"("+ processSubSelect(value.value<QSharedPointer<Select>>(),engine, parameterContainer) + ")");
         }else if(value.userType() == qMetaTypeId<QSharedPointer<AbstractExpression>>()){
            // recursive call to satisfy nested expressions
            strValues.insert(vIndex, processExpression(value.value<QSharedPointer<AbstractExpression>>(), engine, 
                                                       parameterContainer, namedParameterPrefix + QString("%1subpart").arg(vIndex)));
         }else if(type == AbstractExpression::TYPE_IDENTIFIER){
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
            strValues.insert(vIndex, value.toString());
         }
      }
      // After looping the values, interpolate them into the sql string
      // (they might be placeholder names, or values)
      
      QString tpl = subParts[0].toString();
      sql += format_str(tpl, strValues);
   }
   return sql;
}

QString AbstractSql::resolveTable(const TableIdentifier &table, const Engine &engine, ParameterContainer *parameterContainer)
{
   return resolveTable(QVariant::fromValue(table), engine, parameterContainer);
}

QString AbstractSql::resolveTable(const QString &table, const Engine &engine, ParameterContainer *parameterContainer)
{
   return resolveTable(TableIdentifier(table), engine, parameterContainer);
}

QString AbstractSql::resolveTable(const QVariant &vtable, const Engine &engine, ParameterContainer *)
{
   //暂时不支持子查询
   QString schema;
   QString tableName;
   if(vtable.userType() == qMetaTypeId<TableIdentifier>()){
      TableIdentifier table = vtable.value<TableIdentifier>();
      schema = table.getSchema();
      tableName = engine.quoteTableName(table.getTable());
   }else if(vtable.type() == QVariant::String){
      tableName = vtable.toString();
      tableName = engine.quoteTableName(tableName);
   }
   
   if(!schema.isEmpty() && !tableName.isEmpty()){
      tableName = engine.quoteTableName(schema)+engine.getIdentifierSeparator()+tableName;
   }
   return tableName;
}

QString AbstractSql::getDecoratorClassName()const
{
   return QString();
}

AbstractSql::~AbstractSql()
{}

}//sql
}//db
}//corelib
}//sn
