#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

#include "mysql_metadata.h"
#include "global/common_funcs.h"
#include <QDebug>

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace source{

using sn::corelib::format_str;

MysqlMetadata::MysqlMetadata(QSharedPointer<engine::Engine> engine)
   : AbstractSource(engine)
{
   
}

void MysqlMetadata::loadSchemaData()
{
   if(!m_schemasData.isEmpty()){
      return;
   }
   QString sql = QString("SELECT %1 FROM %2 WHERE %3 != 'INFORMATION_SCHEMA'").arg(
            m_engine->quoteFieldName("SCHEMA_NAME"), m_engine->quoteIdentifierChain({"INFORMATION_SCHEMA", "SCHEMATA"}),
            m_engine->quoteFieldName("SCHEMA_NAME"));
   QSharedPointer<QSqlQuery> query = m_engine->query(sql);
   while(query->next()){
      m_schemasData.append(query->value("SCHEMA_NAME").toString());
   }
}

void MysqlMetadata::loadTableNameData(const QString &schema)
{
   if(m_schemasTablesData.contains(schema)){
      return;
   }
   QStringList isColumns;
   isColumns.append(m_engine->quoteIdentifierChain({"T", "TABLE_NAME"}));
   isColumns.append(m_engine->quoteIdentifierChain({"T", "TABLE_TYPE"}));
   isColumns.append(m_engine->quoteIdentifierChain({"V", "VIEW_DEFINITION"}));
   isColumns.append(m_engine->quoteIdentifierChain({"V", "CHECK_OPTION"}));
   isColumns.append(m_engine->quoteIdentifierChain({"V", "IS_UPDATABLE"}));
   QString sql = QString("SELECT %1 FROM %2 LEFT JOIN %3 ON %4 = %5 AND %6 = %7 WHERE %8 IN ('BASE TABLE', 'VIEW')")
         .arg(isColumns.join(", "))
         .arg(m_engine->quoteIdentifierChain({"INFORMATION_SCHEMA", "TABLES"}) + " T")
         .arg(m_engine->quoteIdentifierChain({"INFORMATION_SCHEMA", "VIEWS"}) + " V")
         .arg(m_engine->quoteIdentifierChain({"T", "TABLE_SCHEMA"}))
         .arg(m_engine->quoteIdentifierChain({"V", "TABLE_SCHEMA"}))
         .arg(m_engine->quoteIdentifierChain({"T", "TABLE_NAME"}))
         .arg(m_engine->quoteIdentifierChain({"V", "TABLE_NAME"}))
         .arg(m_engine->quoteIdentifierChain({"T", "TABLE_TYPE"}));
   if(schema != AbstractSource::__DEFAULT_SCHEMA__){
      sql += " AND " + m_engine->quoteIdentifierChain({"T", "TABLE_SCHEMA"}) + 
            " = " + m_engine->quoteValue(schema);
   }else{
      sql += " AND " + m_engine->quoteIdentifierChain({"T", "TABLE_SCHEMA"}) + 
            " != 'INFORMATION_SCHEMA'";
   }
   QSharedPointer<QSqlQuery> query = m_engine->query(sql);
   QMap<QString, QMap<QString, QString>> tables;
   while(query->next()){
      tables.insert(query->value("TABLE_NAME").toString(), 
      {
                       {"table_type", query->value("TABLE_TYPE").toString()},
                       {"view_definition", query->value("VIEW_DEFINITION").toString()},
                       {"check_option", query->value("CHECK_OPTION").toString()},
                       {"is_updatable", query->value("IS_UPDATABLE").toString() == "YES" ? "YES" : "NO"}
                    });
      
   }
   m_schemasTablesData.insert(schema, tables);
}

void MysqlMetadata::loadColumnData(const QString &table, const QString &schema)
{
   if(m_tableColumnsData.contains(schema) && m_tableColumnsData[schema].contains(table)){
      return;
   }
   QStringList isColumns;
   isColumns.append(m_engine->quoteIdentifierChain({"C", "ORDINAL_POSITION"}));
   isColumns.append(m_engine->quoteIdentifierChain({"C", "COLUMN_DEFAULT"}));
   isColumns.append(m_engine->quoteIdentifierChain({"C", "IS_NULLABLE"}));
   isColumns.append(m_engine->quoteIdentifierChain({"C", "DATA_TYPE"}));
   isColumns.append(m_engine->quoteIdentifierChain({"C", "CHARACTER_MAXIMUM_LENGTH"}));
   isColumns.append(m_engine->quoteIdentifierChain({"C", "CHARACTER_OCTET_LENGTH"}));
   isColumns.append(m_engine->quoteIdentifierChain({"C", "NUMERIC_PRECISION"}));
   isColumns.append(m_engine->quoteIdentifierChain({"C", "NUMERIC_SCALE"}));
   isColumns.append(m_engine->quoteIdentifierChain({"C", "COLUMN_NAME"}));
   isColumns.append(m_engine->quoteIdentifierChain({"C", "COLUMN_TYPE"}));
   QStringList args;
   args.append(isColumns.join(", "));
   args.append(m_engine->quoteIdentifierChain({"INFORMATION_SCHEMA", "TABLES"}) + " T");
   args.append(m_engine->quoteIdentifierChain({"INFORMATION_SCHEMA", "COLUMNS"}) + " C");
   args.append(m_engine->quoteIdentifierChain({"T", "TABLE_SCHEMA"}));
   args.append(m_engine->quoteIdentifierChain({"C", "TABLE_SCHEMA"}));
   args.append(m_engine->quoteIdentifierChain({"T", "TABLE_NAME"}));
   args.append(m_engine->quoteIdentifierChain({"C", "TABLE_NAME"}));
   args.append(m_engine->quoteIdentifierChain({"T", "TABLE_TYPE"}));
   args.append(m_engine->quoteIdentifierChain({"T", "TABLE_NAME"}));
   args.append(m_engine->quoteValue(table));
   QString sql = format_str("SELECT %1 FROM %2 INNER JOIN %3 ON %4 = %5 AND %6 = %7 "
                            "WHERE %8 IN ('BASE TABLE', 'VIEW') AND %9 = %10", args);
   if(schema != AbstractSource::__DEFAULT_SCHEMA__){
      sql += " AND " + m_engine->quoteIdentifierChain({"T", "TABLE_SCHEMA"}) + 
            " = " + m_engine->quoteValue(schema);
   }else{
      sql += " AND " + m_engine->quoteIdentifierChain({"T", "TABLE_SCHEMA"}) + 
            " != 'INFORMATION_SCHEMA'";
   }
   QMap<QString, QMap<QString, QVariant>> columns;
   QSharedPointer<QSqlQuery> query = m_engine->query(sql);
   while(query->next()){
      QString columnType = query->value("COLUMN_TYPE").toString();
      QMap<QString, QVariant> erratas;
      QRegularExpression regex("^(?:enum|set)\\((.+)\\)$", QRegularExpression::CaseInsensitiveOption);
      QRegularExpressionMatch match = regex.match(columnType);
      if(match.hasMatch()){
         QString permittedValuesStr = match.captured(1);
         QStringList permittedValues;
         QRegularExpression regex1("\\s*'((?:[^']++|'')*+)'\\s*(?:,|\\$)*", QRegularExpression::CaseInsensitiveOption);
         QRegularExpressionMatchIterator regexIterator = regex1.globalMatch(permittedValuesStr);
         while(regexIterator.hasNext()){
            QRegularExpressionMatch m = regexIterator.next();
            permittedValues.append(m.captured(1).replace("''", "'"));
         }
         erratas.insert("permitted_values", permittedValues);
      }
      QMap<QString, QVariant> columnItem;
      columnItem.insert("ordinal_position", query->value("ORDINAL_POSITION"));
      columnItem.insert("column_default", query->value("COLUMN_DEFAULT"));
      columnItem.insert("is_nullable", query->value("IS_NULLABLE").toString() == "YES");
      columnItem.insert("data_type", query->value("DATA_TYPE"));
      columnItem.insert("character_maximum_length", query->value("CHARACTER_MAXIMUM_LENGTH"));
      columnItem.insert("character_octet_length", query->value("CHARACTER_OCTET_LENGTH"));
      columnItem.insert("numeric_precision", query->value("NUMERIC_PRECISION"));
      columnItem.insert("numeric_scale", query->value("NUMERIC_SCALE"));
      columnItem.insert("numeric_unsigned", -1 != query->value("COLUMN_TYPE").toString().indexOf("unsigned"));
      columnItem.insert("erratas", erratas);
      columns.insert(query->value("COLUMN_NAME").toString(), columnItem);
   }
   m_tableColumnsData[schema].insert(table, columns);
}

void MysqlMetadata::loadConstraintData(const QString &table, const QString &schema)
{
   if(m_tableConstraintData.contains(schema) && m_tableConstraintData[schema].contains(table)){
      return;
   }
   QStringList isColumns;
   isColumns.append(m_engine->quoteIdentifierChain({"T", "TABLE_NAME"}));
   isColumns.append(m_engine->quoteIdentifierChain({"TC", "CONSTRAINT_NAME"}));
   isColumns.append(m_engine->quoteIdentifierChain({"TC", "CONSTRAINT_TYPE"}));
   isColumns.append(m_engine->quoteIdentifierChain({"KCU", "COLUMN_NAME"}));
   isColumns.append(m_engine->quoteIdentifierChain({"RC", "MATCH_OPTION"}));
   isColumns.append(m_engine->quoteIdentifierChain({"RC", "UPDATE_RULE"}));
   isColumns.append(m_engine->quoteIdentifierChain({"RC", "DELETE_RULE"}));
   isColumns.append(m_engine->quoteIdentifierChain({"KCU", "REFERENCED_TABLE_SCHEMA"}));
   isColumns.append(m_engine->quoteIdentifierChain({"KCU", "REFERENCED_TABLE_NAME"}));
   isColumns.append(m_engine->quoteIdentifierChain({"KCU", "REFERENCED_COLUMN_NAME"}));
   
   QStringList args;
   args.append(isColumns.join(", "));
   args.append(m_engine->quoteIdentifierChain({"INFORMATION_SCHEMA", "TABLES"}) + " T");
   args.append(m_engine->quoteIdentifierChain({"INFORMATION_SCHEMA", "TABLE_CONSTRAINTS"}) + " TC");
   args.append(m_engine->quoteIdentifierChain({"T", "TABLE_SCHEMA"}));
   args.append(m_engine->quoteIdentifierChain({"TC", "TABLE_SCHEMA"}));
   args.append(m_engine->quoteIdentifierChain({"T", "TABLE_NAME"}));
   args.append(m_engine->quoteIdentifierChain({"TC", "TABLE_NAME"}));
   args.append(m_engine->quoteIdentifierChain({"INFORMATION_SCHEMA", "KEY_COLUMN_USAGE"}) + " KCU");
   args.append(m_engine->quoteIdentifierChain({"TC", "TABLE_SCHEMA"}));
   args.append(m_engine->quoteIdentifierChain({"KCU", "TABLE_SCHEMA"}));
   args.append(m_engine->quoteIdentifierChain({"TC", "TABLE_NAME"}));
   args.append(m_engine->quoteIdentifierChain({"KCU", "TABLE_NAME"}));
   args.append(m_engine->quoteIdentifierChain({"TC", "CONSTRAINT_NAME"}));
   args.append(m_engine->quoteIdentifierChain({"KCU", "CONSTRAINT_NAME"}));
   args.append(m_engine->quoteIdentifierChain({"INFORMATION_SCHEMA", "REFERENTIAL_CONSTRAINTS"}) + " RC");
   args.append(m_engine->quoteIdentifierChain({"TC", "CONSTRAINT_SCHEMA"}));
   args.append(m_engine->quoteIdentifierChain({"RC", "CONSTRAINT_SCHEMA"}));
   args.append(m_engine->quoteIdentifierChain({"TC", "CONSTRAINT_NAME"}));
   args.append(m_engine->quoteIdentifierChain({"RC", "CONSTRAINT_NAME"}));
   args.append(m_engine->quoteIdentifierChain({"T", "TABLE_NAME"}));
   args.append(m_engine->quoteValue(table));
   args.append(m_engine->quoteIdentifierChain({"T", "TABLE_TYPE"}));
   
   QString sql = format_str("SELECT %1 FROM %2 INNER JOIN %3 ON %4 = %5 AND %6 = %7 "
                            "LEFT JOIN %8 ON %9 = %10 AND %11 = %12 AND %13 = %14 "
                            "LEFT JOIN %15 ON %16 = %17 AND %18 = %19 "
                            "WHERE %20 = %21 AND %22 IN ('BASE TABLE', 'VIEW')", args);
   if(schema != AbstractSource::__DEFAULT_SCHEMA__){
      sql += " AND " + m_engine->quoteIdentifierChain({"T", "TABLE_SCHEMA"}) + 
            " = " + m_engine->quoteValue(schema);
   }else{
      sql += " AND " + m_engine->quoteIdentifierChain({"T", "TABLE_SCHEMA"}) + 
            " != 'INFORMATION_SCHEMA'";
   }
   sql += QString(" ORDER BY CASE %1 WHEN 'PRIMARY KEY' THEN 1 WHEN 'UNIQUE' THEN 2 "
                  "WHEN 'FOREIGN KEY' THEN 3  ELSE 4 END , %2 , %3")
         .arg(m_engine->quoteIdentifierChain({"TC", "CONSTRAINT_TYPE"}))
         .arg(m_engine->quoteIdentifierChain({"TC", "CONSTRAINT_NAME"}))
         .arg(m_engine->quoteIdentifierChain({"KCU", "ORDINAL_POSITION"}));
   
   QSharedPointer<QSqlQuery> query = m_engine->query(sql);
   QMap<QString, QMap<QString, QVariant>> constraints;
   QString realName;
   QString name;
   bool isFK = false;
   while(query->next()){
      if(realName != query->value("CONSTRAINT_NAME").toString()){
         realName = query->value("CONSTRAINT_NAME").toString();
         isFK = "FOREIGN KEY" == query->value("CONSTRAINT_TYPE").toString();
         if(isFK){
            name = realName;
         }else{
            name = "_zf_" + query->value("TABLE_NAME").toString() + "_" + realName;
         }
         QMap<QString, QVariant> constraint;
         constraint.insert("constraint_name", name);
         constraint.insert("constraint_type", query->value("CONSTRAINT_TYPE"));
         constraint.insert("table_name", query->value("TABLE_NAME"));
         constraint.insert("columns", QStringList{});
         if(isFK){
            constraint.insert("referenced_table_schema", name);
            constraint.insert("referenced_table_name", query->value("CONSTRAINT_TYPE"));
            constraint.insert("referenced_columns", QStringList{});
            constraint.insert("match_option", query->value("MATCH_OPTION"));
            constraint.insert("update_rule", query->value("UPDATE_RULE"));
            constraint.insert("delete_rule", query->value("DELETE_RULE"));
         }
         constraints.insert(name, constraint);
      }
      QStringList columnNames(constraints[name]["columns"].toStringList());
      columnNames.append(query->value("COLUMN_NAME").toString());
      constraints[name]["columns"].setValue(columnNames);
      if(isFK){
         QStringList rfcolumnNames(constraints[name]["referenced_columns"].toStringList());
         rfcolumnNames.append(query->value("REFERENCED_COLUMN_NAME").toString());
         constraints[name]["referenced_columns"].setValue(rfcolumnNames);
      }
   }
   m_tableConstraintData[schema].insert(table, constraints);
}

}//source
}//metadata
}//db
}//corelib
}//sn