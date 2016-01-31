#include <algorithm>
#include <QRegularExpression>
#include <QDebug>

#include "create_table_decorator.h"
#include "db/sql/ddl/column/abstract_column.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{
namespace mysql{

using sn::corelib::db::sql::ddl::AbstractColumn;

AbstractSql::ProcessResultPointerType create_table_decorator_process_columns(
      AbstractSql *self,const Engine &engine, 
      ParameterContainer*, QMap<QString, QString>&, 
      QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   CreateTableDecorator* createTableSql = dynamic_cast<CreateTableDecorator*>(self);
   Q_ASSERT_X(createTableSql != 0, "friend function create_table_decorator_process_columns", "self pointer cast fail");
   if(0 == createTableSql){
      throw ErrorInfo(QString("friend function create_table_decorator_process_columns self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(createTableSql->m_columns.isEmpty()){
      result->isNull = true;
      return result;
   }
   QList<QVariant> sqls;
   std::for_each(createTableSql->m_columns.cbegin(), createTableSql->m_columns.cend(), 
                 [&sqls, &createTableSql, &engine](QSharedPointer<AbstractColumn> column)
   {
      QString sql(createTableSql->processExpression(column, engine));
      QMap<int, int> insertStart = createTableSql->getSqlInsertOffsets(sql);
      const QMap<QString, QVariant> &options = column->getOptions();
      QStringList optKeys = options.keys();
      std::sort(optKeys.begin(), optKeys.end(), [&createTableSql](QString left, QString right)->bool{
         left = createTableSql->normalizeColumnOption(left);
         int leftValue = createTableSql->m_columnOptionSortOrder.contains(left) 
               ? createTableSql->m_columnOptionSortOrder.value(left)
               : createTableSql->m_columnOptionSortOrder.size();
         right = createTableSql->normalizeColumnOption(right);
         int rightValue = createTableSql->m_columnOptionSortOrder.contains(right) 
               ? createTableSql->m_columnOptionSortOrder.value(right)
               : createTableSql->m_columnOptionSortOrder.size();
         return leftValue < rightValue;
      });
      int j = 0;
      for(const QString &optKey : optKeys){
         QString insert;
         const QVariant &value = options[optKey];
         if(value.isNull()){
            continue;
         }
         if(optKey == "unsigned"){
            insert = " UNSIGNED";
            j = 0;
         }else if(optKey == "zerofill"){
            insert = " ZEROFILL";
            j = 0;
         }else if(optKey == "identity" || optKey == "serial" || optKey == "autoincrement"){
            insert = " AUTO_INCREMENT";
            j = 1;
         }else if(optKey == "comment"){
            insert = " COMMENT " + engine.quoteValue(value);
            j = 2;
         }else if(optKey == "columnformat" || optKey == "format"){
            insert = " COLUMN_FORMAT " + value.toString().toUpper();
            j = 2;
         }else if(optKey == "storage"){
            insert = " STORAGE " + value.toString().toUpper();
            j = 2;
         }
         
         if(!insert.isEmpty()){
            sql.insert(insertStart[j], insert);
            int insertStartCount = insertStart.size();
            for(; j < insertStartCount; ++j){
               insertStart[j] += insert.size();
            }
         }
      }
      sqls.append(sql);
   });
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   result->value.setValue(QList<QVariant>{
                             sqls
                          });
   return result;
}

CreateTableDecorator::CreateTableDecorator(const QString &table, const QString &schema)
   : CreateTableDecorator(TableIdentifier(table, schema))
{
}

CreateTableDecorator::CreateTableDecorator(const TableIdentifier &table)
   : CreateTable(table),
     m_columnOptionSortOrder(
{
{"unsigned", 0},
{"zerofill", 1},
{"identity", 2},
{"serial", 2},
{"autoincrement", 2},
{"comment", 3},
{"columnformat", 4},
{"format", 4},
{"storage", 5}
})
{
   m_specificationFnPtrs.insert(CreateTable::COLUMNS, create_table_decorator_process_columns);
   m_isNeedLocalizeVariables = true;
}

QMap<int, int> CreateTableDecorator::getSqlInsertOffsets(const QString &sql)
{
   int sqlLength = sql.size();
   QMap<int, int> insertStart;
   QStringList searchWords{
      "NOT NULL", "NULL", "DEFAULT", "UNIQUE", "PRIMARY", "REFERENCES"
   };
   for(const QString& keyword : searchWords){
      int insertPos = sql.indexOf(" " + keyword);
      //潜在的错误地方
      if(-1 != insertPos){
         if("REFERENCES" == keyword){
            if(!insertStart.contains(2)){
               insertStart[2] = insertPos;
            }else{
               insertStart[2] = insertStart[2];
            }
         }else if("PRIMARY" == keyword || "UNIQUE" == keyword){
            if(!insertStart.contains(1)){
               insertStart[1] = insertPos;
            }else{
               insertStart[1] = insertStart[1];
            }
         }else{
            if(!insertStart.contains(0)){
               insertStart[0] = insertPos;
            }else{
               insertStart[0] = insertStart[0];
            }
         }
      }
   }
   for(int i = 0; i <= 3; i++){
      if(!insertStart.contains(i)){
         insertStart[i] = sqlLength;
      }
   }
   return insertStart;
}

QString CreateTableDecorator::normalizeColumnOption(const QString &name)
{
   QString ret(name);
   return ret.replace(QRegularExpression("[-_\\s]"), "").toLower();
}

}//mysql
}//platform
}//sql
}//db
}//corelib
}//sn
