#include <QMap>
#include <QList>

#include "select.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

const  QString Select::SELECT = "select";
const  QString Select::QUANTIFIER = "quantifier";
const  QString Select::COLUMNS = "columns";
const  QString Select::TABLE = "table";
const  QString Select::JOINS = "joins";
const  QString Select::WHERE = "where";
const  QString Select::GROUP = "group";
const  QString Select::HAVING = "having";
const  QString Select::ORDER = "order";
const  QString Select::LIMIT = "limit";
const  QString Select::OFFSET = "offset";
const  QString Select::QUANTIFIER_DISTINCT = "DISTINCT";
const  QString Select::QUANTIFIER_ALL = "ALL";
const  QString Select::JOIN_INNER = "inner";
const  QString Select::JOIN_OUTER = "outer";
const  QString Select::JOIN_LEFT = "left";
const  QString Select::JOIN_RIGHT = "right";
const  QString Select::JOIN_OUTER_RIGHT = "outer right";
const  QString Select::JOIN_OUTER_LEFT  = "outer left";
const  QString Select::SQL_STAR = "*";
const  QString Select::ORDER_ASCENDING = "ASC";
const  QString Select::ORDER_DESCENDING = "DESC";
const  QString Select::COMBINE = "combine";
const  QString Select::COMBINE_UNION = "union";
const  QString Select::COMBINE_EXCEPT = "except";
const  QString Select::COMBINE_INTERSECT = "intersect";


AbstractSql::ProcessResultPointerType process_select(AbstractSql *self,const Engine &engine, 
                                               ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                               QMap<QString, AbstractSql::ProcessResultPointerType> &parameters)
{
   int expr = 1;
   Select* selectSql = qobject_cast<Select*>(self);
   QPair<QString, QString> tablePair = selectSql->resolveTable(selectSql->m_table, engine, parameterContainer);
   QString tableName(tablePair.first);
   QString fromTableName(tablePair.second);
   // process table columns
   QStringList columns;
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   QList<QVariant> values;
   if(tableName.isEmpty()){
      values << QVariant(columns);
   }else{
      values << QVariant(columns) << QVariant(tableName);
   }
   result->value = QVariant(values);
   return result;
}

Select::Select(const QString &table, const QString &schema)
   : Select(TableIdentifier(table, schema))
{
   
}

Select::Select(const TableIdentifier &table)
   : m_table(table)
{
//   m_specifications.insert("statementStart", "%1");
   //select
   QMap<QString, QVariant> selectSpecification;
   {
      QMap<QString, QVariant> part{
         {"1" , "%1"},
         {"2", "%1 AS %2"},
         {"combinedby", ", "}
      };
      QList<QVariant> selectSpecificationItem{
         QVariant(part),
               QVariant() 
      };
      selectSpecification.insert("SELECT %1 FROM %2", QVariant(selectSpecificationItem));
   }
   {
      QMap<QString, QVariant> part{
         {"1" , "%1"},
         {"2", "%1 AS %2"},
         {"combinedby", ", "}
      };
      QList<QVariant> selectSpecificationItem{
         QVariant(),
               QVariant(part),
               QVariant() 
      };
      selectSpecification.insert("SELECT %1 %2 FROM %3", QVariant(selectSpecificationItem));
   }
   {
      QMap<QString, QVariant> part{
         {"1" , "%1"},
         {"2", "%1 AS %2"},
         {"combinedby", ", "}
      };
      QList<QVariant> selectSpecificationItem{
         QVariant(part)
      };
      selectSpecification.insert("SELECT %1", QVariant(selectSpecificationItem));
   }
   m_specifications.insert(Select::SELECT, QVariant(selectSpecification));
   //joins
   QMap<QString, QVariant> joinSpecification;
   {
      QMap<QString, QVariant> part{
         {"3" , "%1 JOIN %2 ON %3"},
         {"combinedby", ", "}
      };
      QList<QVariant> joinSpecificationItem{
         QVariant(part)
      };
      joinSpecification.insert("%1", QVariant(joinSpecificationItem));
   }
//   m_specifications.insert(Select::JOINS, QVariant(joinSpecification));
//   m_specifications.insert(Select::WHERE, QVariant("WHERE %1"));
   //group
   QMap<QString, QVariant> groupSpecification;
   {
      QMap<QString, QVariant> part{
         {"1" , "%1"},
         {"combinedby", ", "}
      };
      QList<QVariant> groupSpecificationItem{
         QVariant(part)
      };
      groupSpecification.insert("GROUP BY %1", QVariant(groupSpecificationItem));
   }
//   m_specifications.insert(Select::GROUP, QVariant(groupSpecification));
//   //having
//   m_specifications.insert(Select::HAVING, QVariant("HAVING %1"));
   //order
   //group
   QMap<QString, QVariant> orderSpecification;
   {
      QMap<QString, QVariant> part{
         {"1" , "%1"},
         {"2", "%1 %2"},
         {"combinedby", ", "}
      };
      QList<QVariant> orderSpecificationItem{
         QVariant(part)
      };
      orderSpecification.insert("ORDER BY %1", QVariant(orderSpecificationItem));
   }
//   m_specifications.insert(Select::ORDER, orderSpecification);
//   m_specifications.insert(Select::LIMIT, QVariant("LIMIT %1"));
//   m_specifications.insert(Select::OFFSET, QVariant("OFFSET %1"));
//   m_specifications.insert("statementEnd", QVariant("%1"));
//   m_specifications.insert(Select::COMBINE, QVariant("%1 ( %2 )"));
   if(!m_table.isNull()){
      m_tableReadOnly = true;
   }
   m_where.reset(new Where());
   m_having.reset(new Having());
   m_specificationFnPtrs.insert("select", process_select);
}

Select& Select::from(const QString &tableName, const QString &schema)throw(ErrorInfo)
{
   return from(TableIdentifier(tableName, schema));
}

Select& Select::from(const TableIdentifier &table)throw(ErrorInfo)
{
   if(m_tableReadOnly){
      throw ErrorInfo("Since this object was created with a table "
                      "and/or schema in the constructor, it is read only.");
   }
   m_table = table;
   return *this;
}

Select& Select::quantifier(const QString &quantifier)
{
   if(!m_quantifier.isNull()){
      m_quantifier.clear();
   }
   m_quantifier.setValue(quantifier);
   return *this;
}

Select& Select::quantifier(const QSharedPointer<AbstractExpression> &quantifier)
{
   if(!m_quantifier.isNull()){
      m_quantifier.clear();
   }
   m_quantifier.setValue(quantifier);
   return *this;
}

QPair<QString, QString> Select::resolveTable(const QString &table, const Engine &engine, ParameterContainer *parameterContainer)
{
   return resolveTable(TableIdentifier(table), engine, parameterContainer);
}

QPair<QString, QString> Select::resolveTable(const TableIdentifier &table, const Engine &engine, ParameterContainer *parameterContainer)
{
   QString tableName = AbstractSql::resolveTable(table, engine, parameterContainer);
   QString fromTableName = tableName;
   if(m_prefixColumnsWithTable && !fromTableName.isEmpty()){
      fromTableName += engine.getIdentifierSeparator();
   }else{
      fromTableName = "";
   }
   return {
      tableName,
      fromTableName
   };
}

QPair<QString, QString> Select::resolveTable(const QString &table, const QString &alias, 
                                             const Engine &engine, engine::ParameterContainer *parameterContainer)
{
   QString tableName = AbstractSql::resolveTable(table, engine, parameterContainer);
   QString fromTableName = tableName;
   if(!alias.isNull() && !alias.isEmpty()){
      fromTableName = engine.quoteTableName(alias);
   }else{
      fromTableName = tableName;
   }
   if(m_prefixColumnsWithTable && !fromTableName.isEmpty()){
      fromTableName += engine.getIdentifierSeparator();
      tableName = renderTable(tableName, fromTableName);
   }else{
      fromTableName = "";
   }
   return {
      tableName,
      fromTableName
   };
}

QString Select::renderTable(const QString &table, const QString &alias)
{
   return table + ((!alias.isNull() && !alias.isEmpty()) ? " AS " + alias : "");
}

}//sql
}//db
}//corelib
}//sn