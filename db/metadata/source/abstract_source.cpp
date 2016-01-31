#include "abstract_source.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace source{

const QString AbstractSource::__DEFAULT_SCHEMA__ = "__DEFAULT_SCHEMA__";

AbstractSource::AbstractSource(QSharedPointer<engine::Engine> engine)
   : m_engine(engine)
{
   QString currentSchema(m_engine->getCurrentSchema());
   if(!currentSchema.isEmpty()){
      m_defaultSchema = currentSchema;
   }else{
      m_defaultSchema = AbstractSource::__DEFAULT_SCHEMA__;
   }
}

const QStringList& AbstractSource::getSchemas()
{
   loadSchemaData();
   return m_schemasData;
}

void AbstractSource::getTableNames(QStringList &tableNames, QString schema, bool includeViews)
{
   if(schema.isEmpty()){
      schema = m_defaultSchema;
   }
   loadTableNameData(schema);
   if(includeViews){
      tableNames = m_schemasTablesData[schema].keys();
      return;
   }
   QMap<QString, QMap<QString, QString>> &schemaTablesData = m_schemasTablesData[schema];
   QMap<QString, QMap<QString, QString>>::const_iterator it = schemaTablesData.cbegin();
   QMap<QString, QMap<QString, QString>>::const_iterator endMarker = schemaTablesData.cend();
   while(it != endMarker){
      if("BASE TABLE" == it.value().value("table_type")){
         tableNames.append(it.key());
      }
      it++;
   }
}


AbstractSource::~AbstractSource()
{}

}//source
}//metadata
}//db
}//corelib
}//sn