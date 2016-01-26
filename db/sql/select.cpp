#include <QMap>
#include <QList>

#include "select.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

const QString Select::SELECT = "select";
const QString Select::QUANTIFIER = "quantifier";
const QString Select::COLUMNS = "columns";
const QString Select::TABLE = "table";
const QString Select::JOINS = "joins";
const QString Select::WHERE = "where";
const QString Select::GROUP = "group";
const QString Select::HAVING = "having";
const QString Select::ORDER = "order";
const QString Select::LIMIT = "limit";
const QString Select::OFFSET = "offset";
const QString Select::QUANTIFIER_DISTINCT = "DISTINCT";
const QString Select::QUANTIFIER_ALL = "ALL";
const QString Select::JOIN_INNER = "inner";
const QString Select::JOIN_OUTER = "outer";
const QString Select::JOIN_LEFT = "left";
const QString Select::JOIN_RIGHT = "right";
const QString Select::JOIN_OUTER_RIGHT = "outer right";
const QString Select::JOIN_OUTER_LEFT = "outer left";
const QString Select::SQL_STAR = "*";
const QString Select::ORDER_ASCENDING = "ASC";
const QString Select::ORDER_DESCENDING = "DESC";
const QString Select::COMBINE = "combine";
const QString Select::COMBINE_UNION = "union";
const QString Select::COMBINE_EXCEPT = "except";
const QString Select::COMBINE_INTERSECT = "intersect";


AbstractSql::ProcessResultPointerType select_process_select(AbstractSql *self,const Engine &engine, 
                                                            ParameterContainer *parameterContainer, QMap<QString, QString>&, 
                                                            QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   int expr = 1;
   Select* selectSql = qobject_cast<Select*>(self);
   QPair<QString, QString> tablePair = selectSql->resolveTable(selectSql->m_table, engine, parameterContainer);
   QString tableName(tablePair.first);
   QString fromTableName(tablePair.second);
   // process table columns
   QList<QVariant> columns;
   if(selectSql->m_columns.isEmpty()){
      selectSql->addColumn(QVariant(Select::SQL_STAR), 0);
   }
   //这样可以吗？
   QList<QVariant> *columnsPointer = &selectSql->m_columns;
   int colCount = columnsPointer->size();
   for(int i = 0; i < colCount; i++){
      QVariant columnIndexOrAs;
      const QVariant &colItem = columnsPointer->at(i);
      QVariant column;
      if(colItem.userType() == qMetaTypeId<QPair<int, QVariant>>()){
         auto pairItem = colItem.value<QPair<int, QVariant>>();
         columnIndexOrAs = pairItem.first;
         column = pairItem.second;
      }else if(colItem.userType() == qMetaTypeId<QPair<QString, QVariant>>()){
         auto pairItem = colItem.value<QPair<QString, QVariant>>();
         columnIndexOrAs = pairItem.first;
         column = pairItem.second;
      }
      if(column.type() == QVariant::String && column.toString() == Select::SQL_STAR){
         columns.append(QVariant(QStringList(fromTableName+Select::SQL_STAR)));
         continue;
      }
      QMap<QString, QVariant> colMeta{
         {"column", column},
         {"fromTable", fromTableName},
         {"isIdentifier", true}
      };
      QString columnName = selectSql->resolveColumnValue(QVariant(colMeta), engine, parameterContainer, 
                                                         columnIndexOrAs.type() == QVariant::String?columnIndexOrAs.toString():"column");
      QString columnAlias;
      if(columnIndexOrAs.type() == QVariant::String){
         columnAlias = engine.quoteFieldName(columnIndexOrAs.toString());
      }else if(columnName.indexOf(" as ") == -1){
         if(column.type() == QVariant::String){
            columnAlias = engine.quoteFieldName(column.toString());
         }else{
            columnAlias = QString("Expression%1").arg(expr++);
         }
      }
      if(columnAlias.isEmpty()){
         columns.append(QVariant(QStringList(columnName)));
      }else{
         columns.append(QVariant(QStringList({columnName, columnAlias})));
      }
   }
   
   QString quantifier;
   //process quantifier
   if(!selectSql->m_quantifier.isNull()){
      if(selectSql->m_quantifier.userType() == qRegisterMetaType<QSharedPointer<AbstractExpression>>()){
         quantifier = selectSql->processExpression(selectSql->m_quantifier.value<QSharedPointer<AbstractExpression>>(), engine, parameterContainer, "quantifier");
      }else if(selectSql->m_quantifier.type() == QVariant::String){
         quantifier = selectSql->m_quantifier.toString();
      }
   }
   
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   QList<QVariant> values;
   //可以返回QList<QVariant> 其中QVariant为 QString 或者 QString
   if(tableName.isEmpty()){
      values << QVariant(columns);
   }else if(!quantifier.isNull() && !quantifier.isEmpty()){
      values << QVariant(quantifier) << QVariant(columns) << QVariant(tableName);
   }else{
      values << QVariant(columns) << QVariant(tableName);
   }
   result->value = QVariant(values);
   return result;
}

AbstractSql::ProcessResultPointerType select_process_where(AbstractSql *self,const Engine &engine, 
                                                           ParameterContainer *parameterContainer, QMap<QString, QString>&, 
                                                           QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Select* selectSql = qobject_cast<Select*>(self);
   Q_ASSERT_X(selectSql != 0, "where friend function process_where", "self pointer cast fail");
   if(0 == selectSql){
      throw ErrorInfo(QString("where friend function process_where self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(selectSql->m_where->count() == 0){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::String;
   result->value = QVariant(QString(selectSql->m_specifications.value(Select::WHERE).toString()).arg(selectSql->processExpression(selectSql->m_where, engine, parameterContainer, "where")));
   return result;
}

AbstractSql::ProcessResultPointerType select_process_having(AbstractSql *self,const Engine &engine, 
                                                            ParameterContainer *parameterContainer, QMap<QString, QString>&, 
                                                            QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Select* selectSql = qobject_cast<Select*>(self);
   Q_ASSERT_X(selectSql != 0, "where friend function process_where", "self pointer cast fail");
   if(0 == selectSql){
      throw ErrorInfo(QString("where friend function process_where self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(selectSql->m_having->count() == 0){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::String;
   result->value = QVariant(QString(selectSql->m_specifications.value(Select::HAVING).toString()).arg(selectSql->processExpression(selectSql->m_having, engine, parameterContainer, "having")));
   return result;
}

AbstractSql::ProcessResultPointerType select_process_group(AbstractSql *self,const Engine &engine, 
                                                           ParameterContainer *parameterContainer, QMap<QString, QString>&, 
                                                           QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Select* selectSql = qobject_cast<Select*>(self);
   Q_ASSERT_X(selectSql != 0, "group friend function process_where", "self pointer cast fail");
   if(0 == selectSql){
      throw ErrorInfo(QString("group friend function process_where self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(selectSql->m_group.size() == 0){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   QList<QVariant> groups;
   std::for_each(selectSql->m_group.cbegin(), selectSql->m_group.cend(), [&groups, &selectSql, &engine, &parameterContainer](const QString &column){
      groups.append(QVariant(selectSql->resolveColumnValue(QMap<QString, QVariant>{
                                                              {"column", column},
                                                              {"isIdentifier", true}
                                                           }, engine, parameterContainer, "group")));
   });
   result->value = QVariant(QList<QVariant>{groups});
   return result;
}

AbstractSql::ProcessResultPointerType select_process_order(AbstractSql *self,const Engine &engine, 
                                                           ParameterContainer*, QMap<QString, QString>&, 
                                                           QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Select* selectSql = qobject_cast<Select*>(self);
   Q_ASSERT_X(selectSql != 0, "group friend function process_where", "self pointer cast fail");
   if(0 == selectSql){
      throw ErrorInfo(QString("group friend function process_where self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(selectSql->m_order.size() == 0){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   QList<QVariant> orders;
   QMap<QString, QString>::const_iterator it = selectSql->m_order.cbegin();
   QMap<QString, QString>::const_iterator endMarker = selectSql->m_order.cend();
   while(it != endMarker){
      orders.append(QVariant(QStringList{
                                engine.quoteIdentifierInFragment(it.key()),
                                it.value()
                             }));
      it++;
   }
   result->value = QVariant(QList<QVariant>{orders});
   return result;
}

AbstractSql::ProcessResultPointerType select_process_limit(AbstractSql *self,const Engine &engine, 
                                                           ParameterContainer* parameterContainer, QMap<QString, QString>&, 
                                                           QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Select* selectSql = qobject_cast<Select*>(self);
   Q_ASSERT_X(selectSql != 0, "limit friend function process_where", "self pointer cast fail");
   if(0 == selectSql){
      throw ErrorInfo(QString("limit friend function process_where self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(selectSql->m_limit.isNull()){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   if(nullptr != parameterContainer){
      parameterContainer->offsetSet("limit", selectSql->m_limit.toInt(), ParameterContainer::TYPE_INTEGER);
      result->value = QVariant(QList<QVariant>{QVariant(engine.formatParameterName("limit"))});
   }else{
      result->value = QVariant(QList<QVariant>{QVariant(QVariant(engine.quoteValue(selectSql->m_limit.toInt())))});
   }
   return result;
}


AbstractSql::ProcessResultPointerType select_process_offset(AbstractSql *self,const Engine &engine, 
                                                           ParameterContainer* parameterContainer, QMap<QString, QString>&, 
                                                           QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Select* selectSql = qobject_cast<Select*>(self);
   Q_ASSERT_X(selectSql != 0, "offset friend function process_where", "self pointer cast fail");
   if(0 == selectSql){
      throw ErrorInfo(QString("offset friend function process_where self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(selectSql->m_offset.isNull()){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   if(nullptr != parameterContainer){
      parameterContainer->offsetSet("offset", selectSql->m_limit.toInt(), ParameterContainer::TYPE_INTEGER);
      result->value = QVariant(QList<QVariant>{QVariant(engine.formatParameterName("offset"))});
   }else{
      result->value = QVariant(QList<QVariant>{QVariant(QVariant(engine.quoteValue(selectSql->m_offset.toInt())))});
   }
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
   m_specifications.insert(Select::WHERE, QVariant("WHERE %1"));
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
   m_specifications.insert(Select::GROUP, QVariant(groupSpecification));
   //having
   m_specifications.insert(Select::HAVING, QVariant("HAVING %1"));
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
   m_specifications.insert(Select::ORDER, orderSpecification);
   m_specifications.insert(Select::LIMIT, QVariant("LIMIT %1"));
   m_specifications.insert(Select::OFFSET, QVariant("OFFSET %1"));
   //   m_specifications.insert("statementEnd", QVariant("%1"));
   //   m_specifications.insert(Select::COMBINE, QVariant("%1 ( %2 )"));
   if(!m_table.isNull()){
      m_tableReadOnly = true;
   }
   m_where.reset(new Where());
   m_having.reset(new Having());
   m_specificationFnPtrs.insert("select", select_process_select);
   m_specificationFnPtrs.insert("where", select_process_where);
   m_specificationFnPtrs.insert("having", select_process_having);
   m_specificationFnPtrs.insert("group", select_process_group);
   m_specificationFnPtrs.insert("order", select_process_order);
   m_specificationFnPtrs.insert("limit", select_process_limit);
   m_specificationFnPtrs.insert("offset", select_process_offset);
   m_specKeys.append("select");
   m_specKeys.append("where");
   m_specKeys.append("having");
   m_specKeys.append("group");
   m_specKeys.append("order");
   m_specKeys.append("limit");
   m_specKeys.append("offset");
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

Select& Select::setQuantifier(const QString &quantifier)
{
   if(!m_quantifier.isNull()){
      m_quantifier.clear();
   }
   m_quantifier.setValue(quantifier);
   return *this;
}

Select& Select::addColumn(const QVariant &column, const QString &alias)
{
   m_columns.append(QVariant::fromValue(QPair<QString, QVariant>(alias, column)));
   return *this;
}

Select& Select::addColumn(const QVariant &column, int index)
{
   m_columns.append(QVariant::fromValue(QPair<int, QVariant>(index, column)));
   return *this;
}

Select& Select::setColumns(QList<QVariant> &columns, bool prefixColumnsWithTable)
{
   //这里是否需要进行检验
   m_columns.clear();
   m_columns = columns;
   m_prefixColumnsWithTable = prefixColumnsWithTable;
   return *this;
}

Select& Select::setPrefixColumnsWithTable(bool flag)
{
   m_prefixColumnsWithTable = flag;
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
      tableName, fromTableName
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
      tableName, fromTableName
   };
}

QString Select::renderTable(const QString &table, const QString &alias)
{
   return table + ((!alias.isNull() && !alias.isEmpty()) ? " AS " + alias : "");
}

Select& Select::where(const QSharedPointer<Where> &where)
{
   m_where = where;
   return *this;
}

Select& Select::where(const QString &where, const QString &combination)
{
   m_where->addPredicate(where, combination);
   return *this;
}

Select& Select::having(const QSharedPointer<Having> &having)
{
   m_having = having;
   return *this;
}

Select& Select::having(const QString &having, const QString &combination)
{
   m_having->addPredicate(having, combination);
   return *this;
}

Select& Select::group(const QString &group)
{
   m_group.append(group);
   return *this;
}

Select& Select::group(const QStringList &groups)
{
   m_group.append(groups);
   return *this;
}

Select& Select::order(const QString &name, const QString &orderType)
{
   if(orderType != Select::ORDER_ASCENDING && orderType != Select::ORDER_DESCENDING){
      m_order.insert(name, Select::ORDER_ASCENDING);
   }else{
      m_order.insert(name, orderType);
   }
   return *this;
}

Select& Select::order(const QMap<QString, QString> &orders)
{
   QMap<QString, QString>::const_iterator it = orders.cbegin();
   QMap<QString, QString>::const_iterator endMarker = orders.cend();
   while(it != endMarker){
      order(it.key(), it.value());
   }
   return *this;
}

Select& Select::limit(quint32 limit)
{
   m_limit.setValue(limit);
   return *this;
}

Select& Select::offset(quint32 offset)
{
   m_offset.setValue(offset);
   return *this;
}

Select& Select::setTableReadOnly(bool flag)
{
   m_tableReadOnly = flag;
   return *this;
}

}//sql
}//db
}//corelib
}//sn