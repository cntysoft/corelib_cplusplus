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

}//source
}//metadata
}//db
}//corelib
}//sn