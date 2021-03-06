#include <QMap>
#include <QList>

#include "select.h"
#include "db/sql/expression.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

using sn::corelib::db::sql::Expression;


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

AbstractSql::ProcessResultPointerType select_process_statement_start(AbstractSql *self,const Engine&, 
                                                                     ParameterContainer*, QMap<QString, QString>&, 
                                                                     QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Select* selectSql = dynamic_cast<Select*>(self);
   Q_ASSERT_X(selectSql != 0, "statement start friend function process_where", "self pointer cast fail");
   if(0 == selectSql){
      throw ErrorInfo(QString("statement start friend function process_where self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(selectSql->m_combine.isEmpty()){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::String;
   result->value = QVariant(QString("("));
   return result;
}

AbstractSql::ProcessResultPointerType select_process_select(AbstractSql *self,const Engine &engine, 
                                                            ParameterContainer *parameterContainer, QMap<QString, QString>&, 
                                                            QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   int expr = 1;
   Select* selectSql = dynamic_cast<Select*>(self);
   Q_ASSERT_X(selectSql != 0, "where friend function select_process_select", "self pointer cast fail");
   if(0 == selectSql){
      throw ErrorInfo(QString("where friend function select_process_select self pointer cast fail"));
   }
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
   //process join columns
   std::for_each(selectSql->m_joins.cbegin(), selectSql->m_joins.cend(), [&self, &selectSql, &columns, &engine, &parameterContainer](const QMap<QString, QVariant> &join){
      QVariant vjoinName = join.value("name");
      QVariant tpmJoinName;
      if(vjoinName.userType() == qMetaTypeId<QPair<QString, QVariant>>()){
         tpmJoinName.setValue(vjoinName.value<QPair<QString, QVariant>>().first);//取alias
      }else{
         tpmJoinName = vjoinName;
      }
      QString joinName = selectSql->resolveTable(tpmJoinName, engine, parameterContainer);
      QMap<QVariant, QVariant> vjoinColumns = join.value("columns").value<QMap<QVariant, QVariant>>();
      QMap<QVariant, QVariant>::const_iterator joinColIterator = vjoinColumns.cbegin();
      QMap<QVariant, QVariant>::const_iterator joinColEndMarker = vjoinColumns.cend();
      while(joinColIterator != joinColEndMarker){
         QStringList jColumns;
         QString jFromTableName;
         QVariant vkey = joinColIterator.key();
         QVariant vColumn = joinColIterator.value();
         if(vkey.type() == QVariant::Int || vkey.type() == QVariant::Bool ||
               vkey.type() == QVariant::UInt || vkey.type() == QVariant::LongLong ||
               vkey.type() == QVariant::ULongLong || vkey.type() == QVariant::Double){
            jFromTableName = joinName + engine.getIdentifierSeparator();
         }else{
            jFromTableName = "";
         }
         QMap<QString, QVariant> colMeta{
            {"column", vColumn},
            {"fromTable", jFromTableName},
            {"isIdentifier", true}
         };
         
         
         jColumns.append(selectSql->resolveColumnValue(QVariant(colMeta), engine, parameterContainer,
                                                       vkey.type() == QVariant::String ? vkey.toString() : "column"));
         if(vkey.type() == QVariant::String){
            jColumns.append(engine.quoteFieldName(vkey.toString()));
         }else if(vColumn.type() == QVariant::String && vColumn.toString() != Select::SQL_STAR){
            jColumns.append(engine.quoteFieldName(vColumn.toString()));
         }
         columns.append(QVariant(jColumns));
         joinColIterator++;
      }
   });
   QString quantifier;
   //process quantifier
   if(!selectSql->m_quantifier.isNull()){
      if(selectSql->m_quantifier.userType() == qMetaTypeId<QSharedPointer<AbstractExpression>>()){
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

AbstractSql::ProcessResultPointerType select_process_join(AbstractSql *self,const Engine &engine, 
                                                          ParameterContainer *parameterContainer, QMap<QString, QString>&, 
                                                          QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Select* selectSql = dynamic_cast<Select*>(self);
   Q_ASSERT_X(selectSql != 0, "join friend function select_process_join", "self pointer cast fail");
   if(0 == selectSql){
      throw ErrorInfo(QString("join friend function select_process_join self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(selectSql->m_joins.size() == 0){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   QList<QVariant> values;
   int joinCount = selectSql->m_joins.size();
   for(int i = 0; i < joinCount; i++){
      const QMap<QString, QVariant> &vjoin = selectSql->m_joins.at(i);
      QVariant vjoinName;
      QString joinAs;
      QVariant vtmpJoinName = vjoin.value("name");
      // table name
      if(vtmpJoinName.userType() == qMetaTypeId<QPair<QString, QVariant>>()){
         QPair<QString, QVariant> namePair = vtmpJoinName.value<QPair<QString, QVariant>>();
         vjoinName = namePair.second;
         joinAs = engine.quoteFieldName(namePair.first);
      }else{
         vjoinName = vtmpJoinName;
      }
      QString joinName;
      if(vjoinName.userType() == qMetaTypeId<QSharedPointer<AbstractExpression>>()){
         joinName = selectSql->processExpression(vjoinName.value<QSharedPointer<AbstractExpression>>(), engine, parameterContainer);
      }else if(vjoinName.userType() == qMetaTypeId<TableIdentifier>()){
         TableIdentifier table = vjoinName.value<TableIdentifier>();
         QPair<QString, QString> tableParts = table.getTableAndSchema();
         if(!tableParts.first.isEmpty()){
            joinName = engine.quoteTableName(tableParts.first) + engine.getIdentifierSeparator();
         }else{
            joinName = "";
         }
         joinName += engine.quoteFieldName(tableParts.second);
      }else if(vjoinName.userType() == qMetaTypeId<QSharedPointer<Select>>()){
         joinName = "(" + selectSql->processSubSelect(vjoinName.value<QSharedPointer<Select>>(), engine, parameterContainer) + ")";
      }else if(vjoinName.type() == QVariant::String){
         joinName = engine.quoteFieldName(vjoinName.toString());
      }
      QStringList joinMetaList{vjoin.value("type").toString().toUpper(), selectSql->renderTable(joinName, joinAs)
                              };
      QVariant von = vjoin.value("on");
      // on expression
      // note: for Expression objects, pass them to processExpression with a prefix specific to each join (used for named parameters)
      if(von.userType() == qMetaTypeId<QSharedPointer<Expression>>()){
         joinMetaList.append(selectSql->processExpression(von.value<QSharedPointer<Expression>>(),engine, 
                                                          parameterContainer, QString("join%1part").arg(i+1)));
      }else{
         joinMetaList.append(engine.quoteIdentifierInFragment(von.toString(), {
                                                                 "=", "AND", "OR", "(", ")", "BETWEEN", "<", ">"
                                                              }));
      }
      values.insert(i, QVariant(joinMetaList));
   }
   result->value = QVariant(QList<QVariant>{QVariant(values)});
   return result;
}

AbstractSql::ProcessResultPointerType select_process_where(AbstractSql *self,const Engine &engine, 
                                                           ParameterContainer *parameterContainer, QMap<QString, QString>&, 
                                                           QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Select* selectSql = dynamic_cast<Select*>(self);
   Q_ASSERT_X(selectSql != 0, "where friend function select_process_where", "self pointer cast fail");
   if(0 == selectSql){
      throw ErrorInfo(QString("where friend function select_process_where self pointer cast fail"));
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
   Select* selectSql = dynamic_cast<Select*>(self);
   Q_ASSERT_X(selectSql != 0, "where friend function select_process_having", "self pointer cast fail");
   if(0 == selectSql){
      throw ErrorInfo(QString("where friend function select_process_having self pointer cast fail"));
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
   Select* selectSql = dynamic_cast<Select*>(self);
   Q_ASSERT_X(selectSql != 0, "group friend function select_process_group", "self pointer cast fail");
   if(0 == selectSql){
      throw ErrorInfo(QString("group friend function select_process_group self pointer cast fail"));
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
   Select* selectSql = dynamic_cast<Select*>(self);
   Q_ASSERT_X(selectSql != 0, "group friend function select_process_order", "self pointer cast fail");
   if(0 == selectSql){
      throw ErrorInfo(QString("group friend function select_process_order self pointer cast fail"));
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
   Select* selectSql = dynamic_cast<Select*>(self);
   Q_ASSERT_X(selectSql != 0, "limit friend function select_process_limit", "self pointer cast fail");
   if(0 == selectSql){
      throw ErrorInfo(QString("limit friend function select_process_limit self pointer cast fail"));
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
      result->value = QVariant(QList<QVariant>{QVariant(engine.quoteValue(selectSql->m_limit.toInt()))});
   }
   return result;
}


AbstractSql::ProcessResultPointerType select_process_offset(AbstractSql *self,const Engine &engine, 
                                                            ParameterContainer* parameterContainer, QMap<QString, QString>&, 
                                                            QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Select* selectSql = dynamic_cast<Select*>(self);
   Q_ASSERT_X(selectSql != 0, "offset friend function select_process_offset", "self pointer cast fail");
   if(0 == selectSql){
      throw ErrorInfo(QString("offset friend function select_process_offset self pointer cast fail"));
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

AbstractSql::ProcessResultPointerType select_process_statement_end(AbstractSql *self,const Engine&, 
                                                                   ParameterContainer*, QMap<QString, QString>&, 
                                                                   QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Select* selectSql = dynamic_cast<Select*>(self);
   Q_ASSERT_X(selectSql != 0, "statement end friend function select_process_statement_end", "self pointer cast fail");
   if(0 == selectSql){
      throw ErrorInfo(QString("statement end friend function select_process_statement_end self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(selectSql->m_combine.isEmpty()){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::String;
   result->value = QVariant(QString(")"));
   return result;
}

AbstractSql::ProcessResultPointerType select_process_combine(AbstractSql *self,const Engine& engine, 
                                                             ParameterContainer* parameterContainer, QMap<QString, QString>&, 
                                                             QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Select* selectSql = dynamic_cast<Select*>(self);
   Q_ASSERT_X(selectSql != 0, "combine friend function select_process_statement_end", "self pointer cast fail");
   if(0 == selectSql){
      throw ErrorInfo(QString("combine friend function select_process_statement_end self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(selectSql->m_combine.isEmpty()){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   QString modifider = selectSql->m_combine.value("modifier").toString();
   QString type = selectSql->m_combine.value("type").toString().toUpper();
   if(!modifider.isNull() && !modifider.isEmpty()){
      type += " " + modifider;
   }
   QString expr(selectSql->processSubSelect(selectSql->m_combine.value("select").value<QSharedPointer<Select>>(), engine, parameterContainer));
   result->value = QVariant(QList<QVariant>{
                               QVariant(type),
                               QVariant(expr)
                            });
   return result;
}

Select::Select(const QString &table, const QString &schema)
   : Select(TableIdentifier(table, schema))
{
}

Select::Select(const TableIdentifier &table)
   : m_table(table)
{
   m_specifications.insert("statementStart", "%1");
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
   m_specifications.insert(Select::JOINS, QVariant(joinSpecification));
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
   m_specifications.insert("statementEnd", QVariant("%1"));
   m_specifications.insert(Select::COMBINE, QVariant("%1 ( %2 )"));
   if(!m_table.isNull()){
      m_tableReadOnly = true;
   }
   m_where.reset(new Where());
   m_having.reset(new Having());
   m_specificationFnPtrs.insert("statementStart", select_process_statement_start);
   m_specificationFnPtrs.insert(Select::SELECT, select_process_select);
   m_specificationFnPtrs.insert(Select::JOINS, select_process_join);
   m_specificationFnPtrs.insert(Select::WHERE, select_process_where);
   m_specificationFnPtrs.insert(Select::HAVING, select_process_having);
   m_specificationFnPtrs.insert(Select::GROUP, select_process_group);
   m_specificationFnPtrs.insert(Select::ORDER, select_process_order);
   m_specificationFnPtrs.insert(Select::LIMIT, select_process_limit);
   m_specificationFnPtrs.insert(Select::OFFSET, select_process_offset);
   m_specificationFnPtrs.insert("statementEnd", select_process_statement_end);
   m_specificationFnPtrs.insert(Select::COMBINE, select_process_combine);
   m_specKeys.append("statementStart");
   m_specKeys.append(Select::SELECT);
   m_specKeys.append(Select::JOINS);
   m_specKeys.append(Select::WHERE);
   m_specKeys.append(Select::HAVING);
   m_specKeys.append(Select::GROUP);
   m_specKeys.append(Select::ORDER);
   m_specKeys.append(Select::LIMIT);
   m_specKeys.append(Select::OFFSET);
   m_specKeys.append("statementEnd");
   m_specKeys.append(Select::COMBINE);
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

Select& Select::addColumn(const QSharedPointer<AbstractExpression> &column, const QString &alias)
{
   m_columns.append(QVariant::fromValue(QPair<QString, QVariant>(alias, QVariant::fromValue(column))));
   return *this;
}

Select& Select::addColumn(const QSharedPointer<AbstractExpression> &column, int index)
{
   m_columns.append(QVariant::fromValue(QPair<int, QVariant>(index, QVariant::fromValue(column))));
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

Select& Select::combine(const QSharedPointer<Select> select, const QString &type, 
                        const QString &modifier)throw(ErrorInfo)
{
   if(!m_combine.isEmpty()){
      throw ErrorInfo("This Select object is already combined and cannot be combined with multiple Selects objects");
   }
   m_combine.insert("select", QVariant::fromValue(select));
   m_combine.insert("type", QVariant(type));
   m_combine.insert("modifier", QVariant(modifier));
   return *this;
}

Select& Select::join(const QString &name, const QString &on, 
                     const QMap<QVariant, QVariant> &columns, const QString &type)
{
   join(QVariant(name), on, columns, type);
   return *this;
}

Select& Select::join(const QVariant &name, const QString &on, 
                     const QMap<QVariant, QVariant> &columns, const QString &type)
{
   QStringList validJoinType{
      Select::JOIN_INNER,
            Select::JOIN_OUTER,
            Select::JOIN_LEFT,
            Select::JOIN_RIGHT,
            Select::JOIN_OUTER_RIGHT,
            Select::JOIN_OUTER_LEFT
   };
   if(!validJoinType.contains(type)){
      return *this;
   }
   m_joins.append({
                     {"name", name},
                     {"on", QVariant(on)},
                     {"columns", QVariant::fromValue(columns)},
                     {"type", QVariant(type)}
                  });
   return *this;
}

Select& Select::join(const QVariant &name, const QString &alias, 
                     const QString &on, const QMap<QVariant, QVariant> &columns, const QString &type)
{
   QStringList validJoinType{
      Select::JOIN_INNER,
            Select::JOIN_OUTER,
            Select::JOIN_LEFT,
            Select::JOIN_RIGHT,
            Select::JOIN_OUTER_RIGHT,
            Select::JOIN_OUTER_LEFT
   };
   if(!validJoinType.contains(type)){
      return *this;
   }
   m_joins.append({
                     {"name", QVariant::fromValue(QPair<QString, QVariant>(alias, name))},
                     {"on", QVariant(on)},
                     {"columns", QVariant::fromValue(columns)},
                     {"type", QVariant(type)}
                  });
   return *this;
}

Select& Select::join(const QSharedPointer<Select> &name, const QString &alias, 
                     const QString &on, const QMap<QVariant, QVariant> &columns, const QString &type)
{
   join(QVariant::fromValue(name), alias, on, columns, type);
   return *this;
}

Select& Select::setSubject(QSharedPointer<Select>)
{
   return *this;
}

Select& Select::setTableReadOnly(bool flag)
{
   m_tableReadOnly = flag;
   return *this;
}

const QSharedPointer<Where>& Select::getWhere()const
{
   return m_where;
}

const QSharedPointer<Having>& Select::getHaving()const
{
   return m_having;
}

Select& Select::reset(const QString &part)throw(ErrorInfo)
{
   if(Select::TABLE == part){
      if(m_tableReadOnly){
         throw ErrorInfo("Since this object was created with a table and/or schema in the constructor, it is read only.");
      }
      m_table.reset();
   }else if(Select::QUANTIFIER == part){
       m_quantifier.clear();
   }else if(Select::COLUMNS == part){
      m_columns.clear();
   }else if(Select::JOINS == part){
      m_joins.clear();
   }else if(Select::WHERE == part){
      m_where.clear();
      m_where.reset(new Where);
   }else if(Select::GROUP == part){
      m_group.clear();
   }else if(Select::HAVING == part){
      m_having.clear();
      m_having.reset(new Having);
   }else if(Select::LIMIT == part){
      m_limit.clear();
   }else if(Select::OFFSET == part){
      m_offset.clear();
   }else if(Select::ORDER == part){
      m_order.clear();
   }else if(Select::COLUMNS == part){
      m_combine.clear();
   }
   return *this;
}

Select::RawState Select::getRawState()const
{
   RawState state;
   state.table = m_table;
   state.tableReadOnly = m_tableReadOnly;
   state.prefixColumnsWithTable = m_prefixColumnsWithTable;
   state.quantifier = m_quantifier;
   state.columns = m_columns;
   state.where = m_where;
   state.having = m_having;
   state.group = m_group;
   state.order = m_order;
   state.limit = m_limit;
   state.offset = m_offset;
   state.combine = m_combine;
   state.joins = m_joins;
   return state;
}

QString Select::getDecoratorClassName()const
{
   return QString("sn::corelib::db::sql::platform::mysql::SelectDecorator");
}

void Select::localizeVariables()
{
   if(!m_isNeedLocalizeVariables || m_subject.isNull()){
      return;
   }
   AbstractSql::localizeVariables();
   QSharedPointer<Select> castedSelectSubject = m_subject.dynamicCast<Select>();
   Q_ASSERT_X(!castedSelectSubject.isNull(), "Select::localizeVariables", "downcast failure");
   m_table = castedSelectSubject->m_table;
   m_tableReadOnly = castedSelectSubject->m_tableReadOnly;
   m_prefixColumnsWithTable = castedSelectSubject->m_prefixColumnsWithTable;
   m_quantifier = castedSelectSubject->m_quantifier;
   m_columns = castedSelectSubject->m_columns;
   m_where = castedSelectSubject->m_where;
   m_having = castedSelectSubject->m_having;
   m_group = castedSelectSubject->m_group;
   m_order = castedSelectSubject->m_order;
   m_limit = castedSelectSubject->m_limit;
   m_offset = castedSelectSubject->m_offset;
   m_combine = castedSelectSubject->m_combine;
   m_joins = castedSelectSubject->m_joins;
}

}//sql
}//db
}//corelib
}//sn