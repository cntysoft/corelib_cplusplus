#include <QSqlQuery>

#include "mysql_metadata.h"
#include <QDebug>

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace source{

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

}//source
}//metadata
}//db
}//corelib
}//sn